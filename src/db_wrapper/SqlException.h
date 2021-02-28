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

#include <exception>
#include <mysql.h>

namespace ourcom
{
class SqlException : public std::exception 
{
public:
	SqlException(MYSQL *c) noexcept :
		message_(mysql_error(c)), code_(mysql_errno(c)) {}

	SqlException(MYSQL_STMT *s) noexcept:
		message_(mysql_stmt_error(s)), code_(mysql_stmt_errno(s)) {}

	virtual unsigned int code() const noexcept 
	{
		return code_;
	}

	virtual const char *what() const noexcept
	{
		return message_; 
	}
private:
	unsigned int code_;
	const char *message_;
};

} // ourcom
