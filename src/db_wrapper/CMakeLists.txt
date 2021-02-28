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

set(src_files SqlConnection.cpp SqlConnectionPool.cpp 
			SqlEnvironment.cpp )
set(header_files SqlConnection.h SqlConnectionPool.h
			SqlEnvironment.h SqlException.h SqlCommand.h SqlReader.h)

add_library(mysqlwrap SHARED ${header_files} ${src_files})

include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(mysqlwrap ${MYSQL_LIB})
