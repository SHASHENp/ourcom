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

#include <vector>
#include <utility>
#include "SqlConnection.h"

class SqlEnvironment;
namespace ourcom 
{
class SqlConnectionPool {
public:
	friend class SqlEnvironment;
	SqlConnectionPool() = default;

	SqlConnectionPool(const SqlConnectionPool&) = delete;
	SqlConnectionPool &operator=(const SqlConnectionPool&) = delete;

	SqlConnection *GetConnection();	
	
	// 释放连接池中所有的连接资源，此后使用此对象获取的连接
	// 指针皆非法
	void TerminateConnectionPool();

	// 将连接归还到连接池中，如果在使用的时候设置了其他状态，
	// 那么恢复成默认状态
	void ReleaseConnection(SqlConnection *conn);
	~SqlConnectionPool();

private:
	void AddConnection(std::shared_ptr<SqlConnection> conn);

	typedef bool flag;
	// flag为true代表可用
	std::vector<std::pair<flag, std::shared_ptr<SqlConnection>>> pool_;
};
} // ourcom 
