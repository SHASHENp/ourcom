/*
 *
 * Copyright 2021 ourcom authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once 

#include <iostream>
#include <mysql.h>
#include <string>
#include <cstring>
#include <cassert>
#include "mysql_exception.h"
#include "prebind_info.h"

namespace ourcom 
{

enum NextState { TLL = 0, OK = 1 };

class SqlReader
{
public: 
	SqlReader(MYSQL_STMT *stmt);

	SqlReader(const SqlReader&) = delete;
	SqlReader &operator=(const SqlReader&) = delete;
	~SqlReader();

	/*
	 * 移动函数，使用Next移动到结果集的下一行或指定的行。但函数并未直接获取一
	 * 行中的所有数据存储起来，而是得到一行中每列的列值长度及是否为null方便后
	 * 续使用GetXXX函数得到实际的数据
	 * 返回值：
	 * 		NextState::OK  成功移动到行
	 * 		NextState::TLL 已经到了最后一行
	 * 
	 * 如果失败则抛出异常
	 */
	NextState Next();

	// 判断指定列的字段是否为空
	bool IsNull(unsigned int column_index)
	{
		return properties_[column_index].is_null;
	}

	/*
	 * 使用Next后，获取相应列对应类型的数据，但如果列结构允许为空注意使用前
	 * 调用IsNull进行判断。函数不以GetXXX(unsigned int, bool *)原型的形式
	 * 是因为虽然这种方式可以提醒用户判断当前列是否为空，但会导致用户编写更多
	 * 的代码。如果列值为NULL，返回随机值
	 */
	int GetInt(unsigned int column_index);
	double GetDouble(unsigned int column_index);
	float GetFloat(unsigned int column_index);
	std::string GetString(unsigned int column_index);

private:
	struct PropertyBind
	{
		my_bool is_null;
		unsigned long length;
	}*properties_{nullptr};

	// 用零长度的缓冲区初始化MYSQL_BIND结构，并设置使用properties的
	// length成员得到实际的长度
	void InitResultBind();

	/*
	 * 根据类型获取对应列数据的通用方法，如果为相应数据为null，那么
	 * 返回随机值。如果查询失败，抛出异常
	 */
	template <typename Ty_> 
	Ty_ GetData(unsigned int column_index);

	// 将存储结果的缓冲区绑定到结构
	void BindResultVariable(unsigned int column_index, char *buffer, 
				unsigned long length, enum_field_types type);
	
private:
	MYSQL_STMT *stmt_{nullptr};
	MYSQL_BIND *result_bind_{nullptr};
	unsigned int column_num_;
	MYSQL_RES *prepare_meta_result_{nullptr};
};

template <typename Ty_> inline
Ty_ SqlReader::GetData(unsigned int column_index)
{
	assert(column_index < column_num_);
	Ty_ data;
	BindResultVariable(column_index, reinterpret_cast<char*>(&data), sizeof(Ty_), 
		PreBind<Ty_>::BufferType());
	if(mysql_stmt_fetch_column(stmt_, &result_bind_[column_index], column_index, 0) != 0)
		throw SqlException(stmt_);
	return data;
}

template <> inline 
std::string SqlReader::GetData<std::string>(unsigned int column_index)
{
	assert(column_index < column_num_);
	if(properties_[column_index].is_null == true)
		return std::string();

	unsigned long data_len = properties_[column_index].length;
	char *p = new char[data_len];
	BindResultVariable(column_index, p, data_len, MYSQL_TYPE_STRING);
	if(mysql_stmt_fetch_column(stmt_, &result_bind_[column_index], column_index, 0) != 0)
	{
		delete[] p;
		throw SqlException(stmt_);
	}
	std::string data(p, data_len);
	delete[] p;
	return data;
}
} // ourcom
