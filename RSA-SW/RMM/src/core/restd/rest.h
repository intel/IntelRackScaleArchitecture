/**
 * Copyright (c)  2015, Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __REST_H__
#define __REST_H__

#include "libutils/list.h"
#include "http.h"
#include "libjson/json.h"
#include "libwrap/wrap.h"
#include "libutils/utils.h"


enum {
	HTTP_SUCCESS = 200,
	HTTP_CREATED = 201,
	HTTP_ACCEPTED = 202,
	HTTP_RESOURCE_INVALID = 400,
	HTTP_UNAUTHORIZED = 401,
	HTTP_RESOURCE_NOT_FOUND = 404,
	HTTP_NOT_ACCEPTABLE = 406,
	HTTP_APPLICATION_ERROR = 500,
};

struct json_result {
	int8   *name;
	json_t *json;
};

struct rest_param_key {
	int8 *name;
	int8 *value;
};

struct rest_uri_param {
	int32 httpmethod;
	int64 content_length;
	int32 fd;
	int32 num_path_keys;
	int32 num_query_keys;
	int8 *json_data;
	uint32 json_data_sz;
	int32 num_nodes;
	struct rest_param_key path_keys[MAX_KEYS_NUM];
	struct rest_param_key query_keys[MAX_KEYS_NUM];
	int8 *nodes[MAX_URI_NODES];
	int8 *host;
	int32 status;
};


struct rest_handler {
	json_t* (*get)(struct rest_uri_param *req);
	json_t* (*post)(struct rest_uri_param *req);
	json_t* (*put)(struct rest_uri_param *req);
};


struct rest_uri_node {
	struct list_head list;
	struct list_head subnode;

	int32  isvar;
	struct rest_handler *rest_handler;
	int8 name[0];
};


extern void rest_register_handlers(void);
extern void register_handler(const int8 *url, const struct rest_handler *rest_handler);
extern void update_response_info(struct rest_uri_param *param, int32 status);
extern void rest_process(struct http_request *req);

extern int8 *rest_path_value(const struct rest_uri_param *param, const int8 *name);
extern int8 *rest_query_value(struct rest_uri_param *param, int8 *name);


#endif
