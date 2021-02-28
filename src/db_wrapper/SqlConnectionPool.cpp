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

#include "SqlConnectionPool.h"
#include "SqlConnection.h"
#include <cassert>
#include <algorithm>

namespace ourcom 
{
void SqlConnectionPool::AddConnection(std::shared_ptr<SqlConnection> conn)
{
	assert(conn != nullptr);
	pool_.push_back(std::make_pair(true, conn));
}

SqlConnection *SqlConnectionPool::GetConnection()
{
	for(auto &elem : pool_)
	{
		if(elem.first == true)
		{
			elem.first = false;
			assert(elem.second != nullptr);
			return elem.second.get();
		}
	}
	return nullptr;
}

void SqlConnectionPool::TerminateConnectionPool()
{
	//for(auto &elem : pool_)
	//	elem.second->TerminateConnection();
	pool_.clear();
}

void SqlConnectionPool::ReleaseConnection(SqlConnection *conn)
{
	if(conn != nullptr)
	{
		if(conn->AutoCommit() == false)
			conn->AutoCommit(true);

		auto iter = find_if(pool_.begin(), pool_.end(), [conn](decltype(pool_)::value_type &value)
				{ return value.second.get() == conn;} );
		if(iter != pool_.end())
			iter->first = true;
	}
}

SqlConnectionPool::~SqlConnectionPool()
{
	TerminateConnectionPool();
}
} // ourcom
