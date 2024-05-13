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

#include <stdint.h>
#include <string>
#include <vector>
#include "ProtocolMessage.h"

/**
 * @file    PostgresMessage.h
 * @brief   PostgreSQL Protocol Interface
 */

namespace protocol
{
class PostgresMessage : public ProtocolMessage
{
public:
	PostgresMessage();
	virtual ~PostgresMessage();
	PostgresMessage(PostgresMessage&& move);
	PostgresMessage& operator= (PostgresMessage&& move);

protected:
	virtual int encode(struct iovec vectors[], int max);
	virtual int append(const void *buf, size_t *size);
};

class PostgresRequest : public PostgresMessage
{
public:
	PostgresRequest() = default;
	PostgresRequest(PostgresRequest&& move) = default;
	PostgresRequest& operator= (PostgresRequest&& move) = default;
};

class PostgresResponse : public PostgresMessage
{
public:
	PostgresResponse() = default;
	PostgresResponse(PostgresResponse&& move) = default;
	PostgresResponse& operator= (PostgresResponse&& move) = default;
};

class PostgresStartupRequest : public PostgresRequest
{
private:
	virtual int encode(struct iovec vectors[], int max) { return 0; }
	PostgresStartupRequest() = default;
	PostgresStartupRequest(PostgresStartupRequest&& move) = default;
	PostgresStartupRequest& operator= (PostgresStartupRequest&& move) = default;

protected:
	virtual int encode(struct iovec vectors[], int max);

private:
	std::string user_;
	std::string password_;
	std::string db_;
	std::string res_charset_;

public:
	void set_conn_info(const std::string& user, const std::string& password,
					   const std::string& db, const std::string& res_charset)
	{
		user_ = user;
		password_ = password;
		db_ = db;
		res_charset_ = res_charset;
	}

};

class PostgresStartupResponse : public PostgresResponse
{
public:
	PostgresStartupResponse() = default;
	PostgresStartupResponse(PostgresStartupResponse&& move) = default;
	PostgresStartupResponse& operator= (PostgresStartupResponse&& move) = default;

protected:
	virtual int append(const void *buf, size_t *size);
};

class PostgresSimpleQueryRequest : public PostgresRequest
{
public:
	PostgresSimpleQueryRequest() = default;
	PostgresSimpleQueryRequest(PostgresSimpleQueryRequest&& move) = default;
	PostgresSimpleQueryRequest& operator= (PostgresSimpleQueryRequest&& move) = default;
};

class PostgresSimpleQueryResponse : public PostgresResponse
{
public:
	PostgresSimpleQueryResponse() = default;
	PostgresSimpleQueryResponse(PostgresSimpleQueryResponse&& move) = default;
	PostgresSimpleQueryResponse& operator= (PostgresSimpleQueryResponse&& move) = default;
};

}