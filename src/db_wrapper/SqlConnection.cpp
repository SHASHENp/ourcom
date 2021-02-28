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

#include "SqlConnection.h"
#include "SqlException.h"
#include "SqlEnvironment.h"

namespace ourcom 
{

void SqlConnection::TerminateConnection()
{
	if(conn_)
	{
		mysql_close(conn_);
		conn_ = nullptr;
	}
}

bool SqlConnection::Normal()
{
	return !mysql_ping(conn_);
}

void SqlConnection::AutoCommit(bool ac)
{
	if(ac != auto_commit_)
	{
		if(mysql_autocommit(conn_, ac) != 0)
			throw SqlException(conn_);
		auto_commit_ = ac;
	}
}

void SqlConnection::Reconnect()
{
	if(!mysql_real_connect(conn_, conn_info_->Host().c_str(), 
				conn_info_->User().c_str(), conn_info_->Passwd().c_str(), 
				conn_info_->Db().c_str(), conn_info_->Port(), NULL, 0))
	{
		throw SqlException(conn_);
	}
}

SqlConnection::~SqlConnection()
{
	TerminateConnection();
}

std::shared_ptr<SqlCommand> SqlConnection::CreateCommand()
{
	return std::make_shared<SqlCommand>(conn_);
}

} // ourcom
