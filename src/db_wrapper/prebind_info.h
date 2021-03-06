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
#include <type_traits>

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

	[[nodiscard]] constexpr Ty_ IsUnsigned() noexcept
	{
		return Ty_();
	}
};

template <>
class PreBind<short> 
{
public:
	PreBind(const short *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return MYSQL_TYPE_SHORT;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<short *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(short);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	const short *member_;
};

template <>
class PreBind<unsigned short> 
{
public:
	PreBind(const unsigned short *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return MYSQL_TYPE_SHORT;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<unsigned short *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(unsigned short);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 1;
	}

private:
	const unsigned short *member_;
};

template <>
class PreBind<int> 
{
public:
	PreBind(const int *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		// c++ 11
		return ( sizeof(short) == sizeof(int) ? MYSQL_TYPE_SHORT : MYSQL_TYPE_LONG);
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<int *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(int);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	const int *member_;
};

template <>
class PreBind<unsigned int> 
{
public:
	PreBind(const unsigned int *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return ( sizeof(short) == sizeof(int) ? MYSQL_TYPE_SHORT : MYSQL_TYPE_LONG);
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<unsigned int *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(unsigned int);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 1;
	}

private:
	const unsigned int *member_;
};

template <>
class PreBind<long> 
{
public:
	PreBind(const long *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return MYSQL_TYPE_LONG;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<long *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(long);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	const long *member_;
};

template <>
class PreBind<unsigned long> 
{
public:
	PreBind(const unsigned long *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return MYSQL_TYPE_LONG;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<unsigned long *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(unsigned long);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 1;
	}

private:
	const unsigned long *member_;
};

template <>
class PreBind<float> 
{
public:
	PreBind(const float *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return MYSQL_TYPE_FLOAT;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<float *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(float);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	const float *member_;
};

template <>
class PreBind<double> 
{
public:
	PreBind(const double *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return MYSQL_TYPE_DOUBLE;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<double *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(double);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	const double *member_;
};

template <>
class PreBind<long long> 
{
public:
	PreBind(const long long *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return MYSQL_TYPE_LONGLONG;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<long long *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(long long);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 0;
	}

private:
	const long long *member_;
};

template <>
class PreBind<unsigned long long> 
{
public:
	PreBind(const unsigned long long *m) :member_(m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return MYSQL_TYPE_LONGLONG;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return reinterpret_cast<char*>(const_cast<unsigned long long *>(member_));
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return sizeof(unsigned long long);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return 1;
	}

private:
	const unsigned long long *member_;
};

template <>
class PreBind<char **> 
{
public:
	PreBind(const char **m) :member_(*m) {}

	[[nodiscard]] static constexpr enum_field_types BufferType() noexcept 
	{
		return MYSQL_TYPE_STRING;
	}
	
	[[nodiscard]] char *Buffer() noexcept 
	{
		return const_cast<char*>(member_);
	}
	
	[[nodiscard]] unsigned long BufferLength() noexcept 
	{
		return strlen(member_);
	}

	[[nodiscard]] constexpr my_bool IsUnsigned() noexcept 
	{
		return std::is_unsigned<char>::value;
	}

private:
	const char *member_;
};

}