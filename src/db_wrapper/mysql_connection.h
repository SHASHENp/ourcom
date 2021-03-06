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

#include <memory>
#include <mysql.h>
#include "mysql_command.h"

namespace ourcom {
	
class ConnectionInformation; 
class SqlConnection
{
public:
	SqlConnection(MYSQL *conn, std::shared_ptr<ConnectionInformation> info) 
		: conn_(conn), conn_info_(info), auto_commit_(true)
	{}
	SqlConnection(const SqlConnection&) = delete;
	SqlConnection &operator=(const SqlConnection&) = delete;

	// 检查数据库连接是否正常
	bool Normal();
	// 重新连接数据库
	void Reconnect();

	void AutoCommit(bool ac);
	bool AutoCommit() const
	{
		return auto_commit_;
	}

	void TerminateConnection();
	
	std::shared_ptr<SqlCommand> CreateCommand();

	~SqlConnection();
private:
	MYSQL *conn_; // 与数据库交互的实例
	bool auto_commit_; // default true
	std::shared_ptr<ConnectionInformation> conn_info_;
};
} // ourcom
