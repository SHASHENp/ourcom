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
#include <memory>
#include <cstring>
#include <string>
#include "mysql_reader.h"
#include "mysql_exception.h"
#include "prebind_info.h"

namespace ourcom
{

class SqlCommand
{
public:
	explicit SqlCommand(MYSQL *conn)
		: conn_(conn)
	{
		if((stmt_ = mysql_stmt_init(conn)) == nullptr)
			throw SqlException(conn); 
	}
	SqlCommand(const SqlCommand&) = delete;
	SqlCommand &operator=(const SqlCommand&) = delete;

	~SqlCommand()
	{
		if(stmt_ != nullptr)
			mysql_stmt_close(stmt_);
	}

	// 设置查询的sql语句
	void Query(const char *sql);

	// 由Query设置的查询命令由此函数执行
	void ExecuteQuery();

	// 直接执行查询的sql语句
	std::shared_ptr<SqlReader> ExecuteQuery(const char *sql);

	/*
	 * ExecuteQuery的模板函数，提供防sql注入的方便写法，支持将内置类型写入到
	 * \param stmt_str执行语句的后继参数中, 对语句进行预处理替换
	 * e.g.
	 *
	 *		std::shared_ptr<ourcom::SqlCommand> command = connection->CreateCommand();      
	 *		command->ExecuteQuery("select * from test where name = ? and age = ?", name, age);
	 */
	template <typename... Args>
	std::shared_ptr<SqlReader> ExecuteQuery(const char *stmt_str, const Args&... args);

	template <typename... Args>
	void SetCommand(const char *fmt, Args&... args);

private:

	/*
	 * 对参数按顺序根据类型信息对MYSQL_BIND结构进行设置,需要确保
	 * MYSQL_BIND数组空间不小于传入的参数个数,目前支持设置以下的
	 * 类型:
	 *		short,  int,  long,  long long,  float,  double,  char *,    
	 *		unsigned short,  unsigned long, unsigned long long
	 *
	 */		
	template <typename T, typename... Args>
	void PrepareBind(MYSQL_BIND *dest, const T &param, const Args&... args);

	template <typename T>
	void PrepareBind(MYSQL_BIND *dest, const T &param);
	
	MYSQL *conn_;
	MYSQL_STMT *stmt_;
	std::string prepared_sql_;
};

inline std::shared_ptr<SqlReader> SqlCommand::ExecuteQuery(const char *sql)
{
	if(mysql_stmt_prepare(stmt_, sql, strlen(sql)) != 0)
		throw SqlException(stmt_);
	if(mysql_stmt_execute(stmt_) != 0)
		throw SqlException(stmt_);
	return std::make_shared<SqlReader>(stmt_);	
}

template <typename... Args>
std::shared_ptr<SqlReader> SqlCommand::ExecuteQuery(const char *stmt_str,const Args&... args)
{
	MYSQL_BIND bind_param[sizeof...(Args)];
	bzero(bind_param, sizeof(bind_param));

	PrepareBind(bind_param, args...);
	if(mysql_stmt_prepare(stmt_, stmt_str, strlen(stmt_str)) != 0)
		throw SqlException(stmt_);
	if(mysql_stmt_bind_param(stmt_, bind_param) != 0)
		throw SqlException(stmt_);
	if(mysql_stmt_execute(stmt_) != 0)
		throw SqlException(stmt_);

	return std::make_shared<SqlReader>(stmt_);
}

template <typename T, typename... Args>
void SqlCommand::PrepareBind(MYSQL_BIND *dest, const T &param, const Args&... args)
{
	PrepareBind(dest, param);
	PrepareBind(++dest, args...);
}

template <typename T>
void SqlCommand::PrepareBind(MYSQL_BIND *dest, const T &param)
{
	PreBind<T> p(&param);

	dest[0].buffer_type = PreBind<T>::BufferType();
	dest[0].buffer = p.Buffer();
	dest[0].buffer_length = p.BufferLength();
	dest[0].is_null = 0;
	dest[0].is_unsigned = p.IsUnsigned();
}

} //ourcom
