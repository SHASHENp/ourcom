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
#include "SqlConnection.h"
#include "SqlConnectionPool.h"
#include <mysql.h>
#include <memory>
#include <string>
#include <list>
#include <iostream>

namespace ourcom {

#define DB_TRY try
#define DB_THROW(ex) throw(ex)
#define DB_CATCH_TIMEOUT() catch()
#define DB_CATCH_ALL() catch(...)

class ConnectionInformationManagement;
ConnectionInformationManagement *GetInfoManagement();

class ConnectionInformation  
{
public:
	friend ConnectionInformationManagement;

	ConnectionInformation()
		: info_manage_(GetInfoManagement())
	{}
	~ConnectionInformation();
	bool operator==(const ConnectionInformation &other);
	bool operator!=(const ConnectionInformation &other);

	std::string Host() const { return host; }
	std::string Db() const { return db; }
	std::string User() const { return db; }
	std::string Passwd() const { return passwd; }

	unsigned int Port() const { return port; }

private:
	std::string host, db;
	std::string user, passwd;
	unsigned int port;

	ConnectionInformationManagement *info_manage_;
};

class ConnectionInformationManagement
{
public:
	void RemoveInformation(ConnectionInformation*);

	std::shared_ptr<ConnectionInformation> AddInformation(const char *host, const char *user,
			const char *passwd, const char *db, unsigned int port);

private:
	std::list<std::weak_ptr<ConnectionInformation> > info_list_;
};
	
class SqlEnvironment
{
public:
	static std::shared_ptr<SqlConnectionPool> CreateConnectionPool(const std::string &host, 
			const std::string &user,const std::string passwd, const std::string &db, 
			unsigned int size, unsigned int port = 3306);

	static std::shared_ptr<SqlConnectionPool> CreateConnectionPool(const char *host, 
			const char *user, const char *passwd, const char *db, 
			unsigned int size, unsigned int port = 3306);
	
	static std::shared_ptr<SqlConnection> CreateConnection(const std::string &host, 
			const std::string &user, const std::string passwd, 
			const std::string &db, unsigned int port = 3306);

	static std::shared_ptr<SqlConnection> CreateConnection(const char *host, const char *user, 
			const char *passwd, const char *db, unsigned int port = 3306);

private:

	static ConnectionInformationManagement *info_manage_;
};

} // ourcom

