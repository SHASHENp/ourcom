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

#include "mysql_reader.h"

namespace ourcom 
{
SqlReader::SqlReader(MYSQL_STMT *stmt)
	: stmt_(stmt)
{
	if((prepare_meta_result_ = mysql_stmt_result_metadata(stmt_)) == NULL)
		throw SqlException(stmt_);
	column_num_ = mysql_num_fields(prepare_meta_result_);
	InitResultBind();
	if(mysql_stmt_bind_result(stmt_, result_bind_) != 0)
		throw SqlException(stmt_);
}

SqlReader::~SqlReader()
{
	delete[] result_bind_;
	delete[] properties_;
}

void SqlReader::InitResultBind()
{
	result_bind_ = new MYSQL_BIND[column_num_];	
	properties_ = new PropertyBind[column_num_];
	bzero(result_bind_, column_num_);
	for(int i = 0; i != column_num_; i++)
	{
		result_bind_[i].is_null = &properties_[i].is_null;
		result_bind_[i].length = &properties_[i].length;
	}
}

NextState SqlReader::Next()
{
	int status = mysql_stmt_fetch(stmt_);
	if(status == MYSQL_NO_DATA)
		return NextState::TLL;
	else if(status != 0 && status != MYSQL_DATA_TRUNCATED)
		throw SqlException(stmt_);

	return NextState::OK;
}

void SqlReader::BindResultVariable(unsigned int column_index, char *buffer, 
		unsigned long length, enum_field_types type)
{
	result_bind_[column_index].buffer = buffer;
	result_bind_[column_index].buffer_length = length;
	result_bind_[column_index].buffer_type = type;
}

int SqlReader::GetInt(unsigned int column_index)
{
	return GetData<int>(column_index);
}

double SqlReader::GetDouble(unsigned int column_index)
{
	return GetData<double>(column_index);
}

float SqlReader::GetFloat(unsigned int column_index)
{
	return GetData<float>(column_index);
}

std::string SqlReader::GetString(unsigned int column_index)
{
	return GetData<std::string>(column_index);
}

} // ourcom
