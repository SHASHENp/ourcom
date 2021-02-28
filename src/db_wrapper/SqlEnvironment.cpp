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

#include <mysql.h>
#include "SqlEnvironment.h"
#include "SqlException.h"

namespace ourcom 
{
ConnectionInformationManagement *SqlEnvironment::info_manage_ = GetInfoManagement();

ConnectionInformationManagement *GetInfoManagement()
{
	static ConnectionInformationManagement info_manage;
	return &info_manage;
}

ConnectionInformation::~ConnectionInformation()
{
	info_manage_->RemoveInformation(this);
	std::cout << "remove this" << std::endl;
}

bool ConnectionInformation::operator==(const ConnectionInformation &other)
{
	return (host == other.host && user == other.user 
				&& passwd == other.passwd && db == other.db 
				&& port == other.port);
}

bool ConnectionInformation::operator!=(const ConnectionInformation &other)
{
	return !(this->operator==(other));
}

void ConnectionInformationManagement::RemoveInformation(
		ConnectionInformation* __attribute__((unused)))
{
	auto iter = info_list_.begin();
	while(iter != info_list_.end())
	{
		if(iter->expired())
			iter = info_list_.erase(iter);
		else 
			++iter;
	}
}

std::shared_ptr<ConnectionInformation> ConnectionInformationManagement::AddInformation(
		const char *host, const char *user, const char *passwd, 
			const char *db, unsigned int port)
{
	std::shared_ptr<ConnectionInformation> info(std::make_shared<ConnectionInformation>());
	info->host = host;
	info->user = user;
	info->passwd = passwd;
	info->db = db;
	info->port = port;
	for(auto &elem : info_list_)
	{
		if(elem.expired() == false)
		{
			auto record = elem.lock();
			if(*record == *info)
				return record;
		}
	}
	info_list_.push_back(info);
	return info;
}

std::shared_ptr<SqlConnectionPool> SqlEnvironment::CreateConnectionPool(
		const std::string &host, const std::string &user,const std::string passwd,
			const std::string &db, unsigned int size, unsigned int port)
{
	return SqlEnvironment::CreateConnectionPool(host.c_str(), user.c_str(), 
			passwd.c_str(), (db.empty() ? nullptr : db.c_str()), size, port);
}

std::shared_ptr<SqlConnectionPool> SqlEnvironment::CreateConnectionPool(
		const char *host, const char *user, const char *passwd, const char *db, 
			unsigned int size, unsigned int port)
{
	std::shared_ptr<SqlConnectionPool> pool(std::make_shared<SqlConnectionPool>());
	for(int i = 0; i < size; i++)
	{
		MYSQL *conn = mysql_init(NULL);
		if(!mysql_real_connect(conn, host, user, passwd, 
				db, port, NULL, 0))
		{
			pool->TerminateConnectionPool();
			throw SqlException(conn);	
		}
		auto info = info_manage_->AddInformation(host, user, passwd, db, port);	
		pool->AddConnection(std::make_shared<SqlConnection>(conn, info));
	}
	return pool;
}

std::shared_ptr<SqlConnection> SqlEnvironment::CreateConnection(
		const std::string &host, const std::string &user, const std::string passwd, 
			const std::string &db, unsigned int port)
{
	return SqlEnvironment::CreateConnection(host.c_str(), user.c_str(),
			passwd.c_str(), (db.empty() ? nullptr : db.c_str()), 3306);
}

std::shared_ptr<SqlConnection> SqlEnvironment::CreateConnection(
		const char *host, const char *user, const char *passwd,
			const char *db, unsigned int port)
{
	MYSQL *conn = mysql_init(NULL);
	if(!mysql_real_connect(conn, host, user, passwd, 
				db, port, NULL, 0))
	{
		throw SqlException(conn);		
	}
	auto info = info_manage_->AddInformation(host, user, passwd, db, port);	
	return std::make_shared<SqlConnection>(conn, info);
}

} // ourcom
