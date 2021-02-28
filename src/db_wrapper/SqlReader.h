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

#include <mysql.h>
#include <string.h>
#include "SqlException.h"

namespace ourcom 
{

enum class NextStatus
{
	OK = 0,
	EOL = 1			// end-of-line
};

class SqlReader
{
public: 
	SqlReader(MYSQL_STMT *stmt);
	~SqlReader();

	// 移动函数
	NextStatus Next();
	NextStatus Next(unsigned int row);

	// 使用Next后，获取相应列对应类型的数据
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
	// length成员存储实际的长度
	void InitResultBind();
	
private:
	MYSQL_STMT *stmt_{nullptr};
	MYSQL_BIND *result_bind_{nullptr};
	unsigned int column_num_;
	MYSQL_RES *prepare_meta_result_{nullptr};
};
} // ourcom
