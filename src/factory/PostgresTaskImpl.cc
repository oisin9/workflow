/*
  Copyright (c) 2019 Sogou, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  Authors: Oisin (oisin-tech@freeoisin.com)
*/

#include <stdio.h>
#include <string.h>
#include <string>
#include <WFTaskError.h>
#include <WFTaskFactory.h>
#include "StringUtil.h"

using namespace protocol;

class ComplexPostgresTask : public WFComplexClientTask<PostgresRequest, PostgresResponse>
{
public:
	ComplexPostgresTask(int retry_max, postgres_callback_t&& callback):
		WFComplexClientTask(retry_max, std::move(callback))
	{}
protected:
	virtual CommMessageOut *message_out();
	virtual CommMessageIn *message_in();
	virtual bool finish_once();
	virtual bool init_success();
	virtual bool check_request();

private:
	bool is_user_request_;
	std::string username_;
	std::string password_;
	std::string dbname_;
	std::string sslmode_;
	std::string application_name_;
	std::string client_encoding_;
	std::string options_;
};

CommMessageOut *ComplexPostgresTask::message_out()
{}

CommMessageIn *ComplexPostgresTask::message_in()
{}

bool ComplexPostgresTask::finish_once()
{}

bool ComplexPostgresTask::init_success()
{
	// TODO: 把uri_字段内username, password, dbname等值存到Task中的相应字段中
	if (uri_.scheme && strcasecmp(uri_.scheme, "postgresql") != 0)
	{
		this->state = WFT_STATE_TASK_ERROR;
		this->error = WFT_ERR_URI_SCHEME_INVALID;
		return false;
	}

	username_.clear();
	password_.clear();
	dbname_.clear();

	if (uri_.path && uri_.path[0] == '/' && uri_.path[1])
	{
		dbname_.assign(uri_.path + 1);
		StringUtil::url_decode(dbname_);
	}

	if (uri_.query)
	{
		auto query_kv = URIParser::split_query(uri_.query);

		for (auto& kv : query_kv)
		{
			if (strcasecmp(kv.first.c_str(), "user") == 0)
				username_ = kv.second;
			else if (strcasecmp(kv.first.c_str(), "password") == 0)
				password_ = kv.second;
			else if (strcasecmp(kv.first.c_str(), "sslmode") == 0)
				sslmode_ = kv.second;
			else if (strcasecmp(kv.first.c_str(), "application_name") == 0)
				application_name_ = kv.second;
			else if (strcasecmp(kv.first.c_str(), "client_encoding") == 0)
				client_encoding_ = kv.second;
			else if (strcasecmp(kv.first.c_str(), "options") == 0)
				options_ = kv.second;
		}
	}

	this->WFComplexClientTask::set_transport_type(TT_TCP);

	size_t info_len = username_.size() + password_.size() + dbname_.size() + 50;
	char *info = new char[info_len];
	sprintf(info, "postgres|user:%s|pass:%s|db:%s", username_.c_str(),
			password_.c_str(), dbname_.c_str());

	this->WFComplexClientTask::set_info(info);
}

bool ComplexPostgresTask::check_request()
{
	// TODO: 检查是否是一个合法的请求
}


/**********Factory**********/

// postgresql://[hostname]:[port]/[database]?[parameters]
// url = "postgresql://localhost:5432/mydatabase?user=postgres&password=strongPasswd&sslmode=verify-full"

WFPostgresTask *WFTaskFactory::create_postgres_task(const std::string& url,
													int retry_max,
													postgres_callback_t callback)
{
	auto *task = new ComplexPostgresTask(retry_max, std::move(callback));
	ParsedURI uri;

	URIParser::parse(url, uri);
	task->init(std::move(uri));
	return task;
}

WFPostgresTask *WFTaskFactory::create_postgres_task(const ParsedURI& uri,
													int retry_max,
													postgres_callback_t callback)
{
	auto *task = new ComplexPostgresTask(retry_max, std::move(callback));

	task->init(uri);
	return task;
}
