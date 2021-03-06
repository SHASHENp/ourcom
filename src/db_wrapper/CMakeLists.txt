#
#
# Copyright 2021 ourcom authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

cmake_minimum_required(VERSION 3.10)

project(mysqlwrap LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(MySQL REQUIRED)

#if(${MYSQL_VERSION})

#endif()

if(NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 11)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(src_files mysql_connection.cpp mysql_connection_pool.cpp 
			mysql_environment.cpp mysql_command.cpp mysql_reader.cpp)
set(header_files mysql_connection.h mysql_connection_pool.h
			mysql_environment.h mysql_exception.h mysql_command.h mysql_reader.h
			prebind_info.h)

add_library(mysqlwrap SHARED ${header_files} ${src_files})

include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(mysqlwrap ${MYSQL_LIB})
