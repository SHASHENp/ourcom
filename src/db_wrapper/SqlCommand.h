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
#include <type_traits>
#include <cstring>
#include "SqlReader.h"
#include "SqlException.h"

namespace ourcom
{

template <typename Ty_>
struct PreBind
{
	[[nodiscard]] static constexpr Ty_ BufferType() noexcept 
	{
		return Ty_();
	}

	[[nodiscard]] Ty_ Buffer() noexcept 
	{
		return Ty_();
	}

	[[nodiscard]] Ty_ BufferLength() noexcept 
	{
		return Ty_();
	}

	[[nodiscard]] Ty_ IsUnsigned() noexcept
	{
		return Ty_();
	}
};

template <>
class PreBind<short> 
{
public:
	PreBind(short *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return MYSQL_TYPE_SHORT;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(short);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	short *member_;
};

template <>
class PreBind<unsigned short> 
{
public:
	PreBind(unsigned short *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return MYSQL_TYPE_SHORT;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(unsigned short);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 1;
	}

private:
	unsigned short *member_;
};

template <>
class PreBind<int> 
{
public:
	PreBind(int *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		// c++ 11
		return ( sizeof(short) == sizeof(int) ? MYSQL_TYPE_SHORT : MYSQL_TYPE_LONG);
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(int);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	int *member_;
};

template <>
class PreBind<unsigned int> 
{
public:
	PreBind(unsigned int *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return ( sizeof(short) == sizeof(int) ? MYSQL_TYPE_SHORT : MYSQL_TYPE_LONG);
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(unsigned int);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 1;
	}

private:
	unsigned int *member_;
};

template <>
class PreBind<long> 
{
public:
	PreBind(long *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return MYSQL_TYPE_LONG;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(long);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	long *member_;
};

template <>
class PreBind<unsigned long> 
{
public:
	PreBind(unsigned long *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return MYSQL_TYPE_LONG;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(unsigned long);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 1;
	}

private:
	unsigned long *member_;
};

template <>
class PreBind<float> 
{
public:
	PreBind(float *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return MYSQL_TYPE_FLOAT;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(float);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	float *member_;
};

template <>
class PreBind<double> 
{
public:
	PreBind(double *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return MYSQL_TYPE_DOUBLE;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(double);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	double *member_;
};

template <>
class PreBind<long long> 
{
public:
	PreBind(long long *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return MYSQL_TYPE_LONGLONG;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(long long);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	long long *member_;
};

template <>
class PreBind<unsigned long long> 
{
public:
	PreBind(unsigned long long *m) :member_(m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return MYSQL_TYPE_LONGLONG;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(unsigned long long);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return 1;
	}

private:
	unsigned long long *member_;
};

template <>
class PreBind<char **> 
{
public:
	PreBind(char **m) :member_(*m) {}

	[[nodiscard]] static constexpr unsigned long BufferType() noexcept 
	{
		return MYSQL_TYPE_STRING;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return strlen(member_);
	}

	[[nodiscard]] my_bool IsUnsigned() noexcept 
	{
		return std::is_unsigned<char>::value;
	}

private:
	char *member_;
};

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
	std::shared_ptr<SqlReader> ExecuteQuery(const char *stmt_str, Args&... args);

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
	void PrepareBind(MYSQL_BIND *dest, T &param, Args&... args);

	template <typename T>
	void PrepareBind(MYSQL_BIND *dest, T &param);
	
	MYSQL *conn_;
	MYSQL_STMT *stmt_;	
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
std::shared_ptr<SqlReader> SqlCommand::ExecuteQuery(const char *stmt_str, Args&... args)
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

template <typename T, typename... Args> inline
void SqlCommand::PrepareBind(MYSQL_BIND *dest, T &param, Args&... args)
{
	PrepareBind(dest, param);
	PrepareBind(++dest, args...);
}

template <typename T> inline
void SqlCommand::PrepareBind(MYSQL_BIND *dest, T &param)
{
	PreBind<T> p(param);

	dest[0].buffer_type = PreBind<T>::BufferType();
	dest[0].buffer = p.Buffer();
	dest[0].buffer_length = p.BufferLength();
	dest[0].is_null = 0;
	dest[0].is_unsigned = p.IsUnsigned();
}

} //ourcom
