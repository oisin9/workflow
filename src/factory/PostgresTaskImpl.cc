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
};

CommMessageOut *ComplexPostgresTask::message_out()
{}

CommMessageIn *ComplexPostgresTask::message_in()
{}

bool ComplexPostgresTask::finish_once()
{}


/**********Factory**********/

// postgresql://[hostname]:[port]/[database]?[parameters]
// url = "postgresql://localhost:5432/mydatabase?user=postgres&password=strongPasswd&sslmode=verify-full"

WFPostgresTask *WFTaskFactory::create_postgres_task(const std::string& url,
													int retry_max,
													postgres_callback_t callback)
{}

WFPostgresTask *WFTaskFactory::create_postgres_task(const ParsedURI& uri,
													int retry_max,
													postgres_callback_t callback)
{}
