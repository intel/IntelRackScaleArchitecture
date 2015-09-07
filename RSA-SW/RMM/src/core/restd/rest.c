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


#include "rest.h"
#include "libjson/json.h"
#include "handler/handler.h"
#include "librmmcfg/rmm_cfg.h"
#include "libutils/string.h"

struct http_response_status {
	int32 status;
	int8 *title;
};

static struct rest_uri_node root_uri_node = {
	.list    = LIST_HEAD_INIT(root_uri_node.list),
	.subnode = LIST_HEAD_INIT(root_uri_node.subnode),

	.isvar   = 0,
	.rest_handler  = NULL,
};

static const struct http_response_status http_resp[] = {
	{HTTP_SUCCESS, "Success"},
	{HTTP_CREATED, "Created"},
	{HTTP_ACCEPTED, "Accepted"},
	{HTTP_RESOURCE_INVALID, "Resource invalid"},
	{HTTP_UNAUTHORIZED, "Unauthorized"},
	{HTTP_RESOURCE_NOT_FOUND, "Resource not found"},
	{HTTP_NOT_ACCEPTABLE, "Not acceptable"},
	{HTTP_APPLICATION_ERROR, "Application Error"},
};


static void compile_rest_uri(int32 num, int8 *ids[], const struct rest_handler *handler);
static int32 split_path(int8 *path, int8 *ids[]);
static int32 parse_query_params(int8 *query, struct rest_param_key *params);
static struct rest_uri_node *create_rest_node(int8 *name,
											  struct list_head *parent);
static void send_json_reply(int32 fd, int32 status, const int8 *title, const int8 *json, int32 jsonlen);
static struct rest_handler *lookup_rest_handle(struct rest_uri_param *param);
static json_t *handle_rest_req(const struct rest_uri_param *param, const struct rest_handler *handler);


void rest_register_handlers(void)
{
	register_rack_handler();
	register_mbp_handler();
	register_pzone_handler();
	register_psu_handler();
	register_drawer_handler();
	register_tzone_handler();
	register_fan_handler();
}

void register_handler(const int8 *url, const struct rest_handler *handler)
{
	int32 num;
	int8 *ids[MAX_URI_NODES];
	int8 path[MAX_HEADER_LEN];
	int8 prefix[MAX_URL] = {0};
	int8 new_link[MAX_URL] = {0};

	rmm_cfg_get_rest_prefix(prefix, MAX_URL);
	insert_str(new_link, url, 0, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/

	strncpy_safe(path, new_link, MAX_HEADER_LEN, MAX_HEADER_LEN - 1);
	num = split_path(path, ids);

	compile_rest_uri(num, ids, handler);
}

int8 *rest_path_value(const struct rest_uri_param *param, const int8 *name)
{
	int32 i;

	for (i = 0; i < param->num_path_keys; i++) {
		if (strcasecmp(param->path_keys[i].name, name) == 0)
			return param->path_keys[i].value;
	}

	return "";
}

int8 *rest_query_value(struct rest_uri_param *param, int8 *name)
{
	int32 i;

	for (i = 0; i < param->num_query_keys; i++) {
		if (strcasecmp(param->query_keys[i].name, name) == 0)
			return param->query_keys[i].value;
	}

	return NULL;
}

void update_response_info(struct rest_uri_param *param, int32 status)
{
	param->status = status;
}

void rest_process(struct http_request *req)
{
	json_t *result = NULL;
	struct rest_handler *handle;
	struct rest_uri_param param;
	int32 i = 0;

	param.httpmethod     = req->method;
	param.content_length = req->content_length;
	param.fd             = req->fd;
	param.num_nodes      = split_path(req->path, param.nodes);
	param.num_query_keys = parse_query_params(req->query, param.query_keys);
	param.status         = HTTP_ACCEPTED;
	param.host           = req->host;

	param.json_data_sz = req->json_end - req->json_start + 1;
	param.json_data = req->buff + req->json_start;

	handle = lookup_rest_handle(&param);
	if (handle != NULL)
		result = handle_rest_req(&param, handle);
	else
		result = NULL;

	if (result != NULL) {
		int32 sz;
		int8 body[100*1024];

		sz = json_format(result, body, sizeof(body));

		for (i = 0; i < sizeof(http_resp)/sizeof(struct http_response_status); i++) {
			if (param.status == http_resp[i].status) {
				send_json_reply(req->fd, http_resp[i].status,
								http_resp[i].title, body, sz);

				json_free(result);
				return;
			}
		}

		send_json_reply(req->fd, 200, "OK", body, sz);
	} else {
		for (i = 0; i < sizeof(http_resp)/sizeof(struct http_response_status); i++) {
			if (param.status == http_resp[i].status) {
				send_json_reply(req->fd, http_resp[i].status,
								http_resp[i].title, NULL, 0);
				return;
			}
		}

		send_json_reply(req->fd, 400, "Invalid REST request", "{}", 2);
	}

	if (NULL != result)
		json_free(result);
}


static int32 split_path(int8 *path, int8 *ids[])
{
	int32 i = 0;
	int8 *cp = path;

	if (*cp != '/')
		return 0;

	for (cp++; i < MAX_URI_NODES;) {
		ids[i] = cp;
		while (*cp != '\0' && *cp != '/')
			cp++;

		i++;

		if (*cp == '\0')	/* end of path */
			break;
		else {		/* *cp is '/' */
			*cp = '\0';	/* replace '/' */
			cp++;

			if (*(cp) == '\0')	/* end of path */
				break;
		}
	}

	return i;
}

static int8 *decode_node(int8 *node, int32 *isvar)
{
	*isvar = 0;

	if (*node == '{') {
		int8 *tmp;

		*isvar = 1;

		tmp = strchr(++node, '}');
		if (tmp != NULL)
			*tmp = '\0';
	}

	return node;
}

static struct rest_uri_node *alloc_rest_uri_node(const int8 *id, int32 isvar)
{
	int32 idlen;
	struct rest_uri_node *node;

	idlen = strlen(id);
	node = malloc(sizeof(*node) + idlen + 1);

	if (node == NULL)
		FATAL("No memory for restd REST, exiting ...\n");

	INIT_LIST_HEAD(&node->list);
	INIT_LIST_HEAD(&node->subnode);

	node->isvar  = isvar;
	node->rest_handler = NULL;

	memcpy(node->name, id, idlen);
	node->name[idlen] = '\0';

	return node;
}

static struct rest_uri_node *create_rest_node(int8 *name,
											  struct list_head *parent)
{
	int32 isvar;
	int8 *id;
	struct rest_uri_node *n;

	id = decode_node(name, &isvar);
	list_for_each_entry(n, parent, list) {
		if (strcasecmp(n->name, id) == 0)
			return n;
	}

	n = alloc_rest_uri_node(id, isvar);
	list_add_tail(&n->list, parent);

	return n;
}

static void compile_rest_uri(int32 num, int8 *ids[], const struct rest_handler *handler)
{
	int32 i;
	struct rest_uri_node *leaf, *parent;

	leaf   = NULL;
	parent = &root_uri_node;
	for (i = 0; i < num; i++) {
		leaf = create_rest_node(ids[i], &parent->subnode);
		parent = leaf;
	}

	if (leaf == NULL || leaf->rest_handler != NULL)
		FATAL("No leaf or Duplicate REST URI path!\n");

	leaf->rest_handler = (struct rest_handler *)handler;
}


static int32 parse_query_params(int8 *query, struct rest_param_key *params)
{
	int32 i;
	int8 *to, *cp = query;

	if (cp == NULL)
		return 0;

	for (i = 0; i < MAX_KEYS_NUM; i++, cp++) {
		params[i].name = cp;
		to = cp;
		while (*cp != '\0' && *cp != '=') {
			if (cp[0] == '%' && isxdigit(cp[1]) && isxdigit(cp[2])) {
				*to++ = (hexit(cp[1]) << 4) | hexit(cp[2]);
				cp += 3;
			} else {
				*to++ = *cp++;
			}
		}
		if (*cp == '\0')
			break;
		*to = '\0';

		params[i].value = ++cp;
		to = cp;
		while (*cp != '\0' && *cp != '&') {
			if (cp[0] == '%' && isxdigit(cp[1]) && isxdigit(cp[2])) {
				*to++ = (hexit(cp[1]) << 4) | hexit(cp[2]);
				cp += 3;
			} else {
				*to++ = *cp++;
			}
		}
		if (*cp == '\0') {
			*to = '\0';
			i++;
			break;
		}
		*to = '\0';

	}

	return i;
}

static void send_json_reply(int32 fd, int32 status, const int8 *title, const int8 *json, int32 jsonlen)
{
	int32 len;
	int8 header[1024];

	len = snprintf(header, sizeof(header),
			"%s %d %s\r\n"
			"Server: %s\r\n"
			"Access-Control-Allow-Origin: *\r\n"
			"Content-Type: application/json\r\n"
			"Access-Control-Allow-Methods: GET,PUT,POST,DELETE\r\n"
			"Access-Control-Allow-Headers: Content-Type\r\n"
			"Proxy-Connection: Keep-Alive\r\n"
			"Connection: close\r\n"
			"\r\n",
			HTTPD_PROTOCOL, status, title, HTTPD_SERVER_NAME);

	http_write(fd, header, len);

	if ((json != NULL) && (jsonlen > 0))
		http_write(fd, json, jsonlen);
	else
		http_write(fd, "{}", 2);
}

static inline struct rest_uri_node *find_rest_node(const int8 *name, struct list_head *parent)
{
	struct rest_uri_node *n, *var = NULL;

	list_for_each_entry(n, parent, list) {
		if (strcasecmp(n->name, name) == 0)
			return n;
		else if (n->isvar == 1)
			var = n;
	}

	return var;
}

static struct rest_handler *lookup_rest_handle(struct rest_uri_param *param)
{
	int32 i;
	int32 num_keys = 0;
	struct rest_uri_node *leaf, *parent;

	leaf   = NULL;
	parent = &root_uri_node;
	for (i = 0; i < param->num_nodes; i++) {
		if ((leaf = find_rest_node(param->nodes[i], &parent->subnode)) == NULL)
			break;

		parent = leaf;
		if (leaf->isvar) {
			param->path_keys[num_keys].name = leaf->name;
			param->path_keys[num_keys].value = param->nodes[i];
			num_keys++;
		}
	}
	param->num_path_keys = num_keys;

	return leaf != NULL ? leaf->rest_handler : NULL;
}

static json_t *handle_rest_req(const struct rest_uri_param *param, const struct rest_handler *handler)
{
	json_t *result = NULL;

	switch (param->httpmethod) {
	case M_GET:
		if (handler->get != NULL)
			result = handler->get((struct rest_uri_param *)param);
		break;
	case M_PUT:
		if (handler->put != NULL)
			result = handler->put((struct rest_uri_param *)param);
		break;
	case M_POST:
		if (handler->post != NULL)
			result = handler->post((struct rest_uri_param *)param);
		break;
	default:
		break;
	}
	return result;
}

