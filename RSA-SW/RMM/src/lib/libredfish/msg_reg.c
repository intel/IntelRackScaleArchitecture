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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "libutils/file.h"
#include "libutils/string.h"
#include "libredfish/msg_reg.h"
#include "libredfish/rf_types.h"
#include "libutils/types.h"

#define TYPE_STR			"string"
#define TYPE_INT			"number"
#define TYPE_STR_ID			1
#define TYPE_INT_ID			2

#define MR_JSON_FILE_PATH	"/var/rmm/redfish/MR.json"

struct type_map rf_evt_params_convet_map[2] = {
	{"%integer",	"%d"},
	{"%string",		"%s"}
};

/**
 * @brief The data pointer of redfish message registry.
 *
 */
static struct message_registry *g_mr;

static int get_args_count(const char *buf, char **args)
{
	char tmp[256] = {0};
	const char *split = ",";
	char *p = NULL;
	int i = 0;

	strncpy_safe(tmp, buf, sizeof(tmp), sizeof(tmp) - 1);
	p = strtok(tmp, split);
	while (p != NULL) {
		args[i] = strdup(p);
		p = strtok(NULL, split);
		i++;
	}

	return i;
}

/*Get message data through the message format("%d,%s,%s") and args*/
static int parse_msg(char *msg, char *msg_fmt, char **args, int count)
{
	char	*str;
	char	*arg;
	int		index = 0;
	int		size = 0;
	int		i = 0;

	for (str = msg; *msg_fmt; ++msg_fmt) {
		if (*msg_fmt != '%') {
			*str++ = *msg_fmt;
			continue;
		}
		msg_fmt++;
		switch (*msg_fmt) {
		case 's':
		case 'd': {
			arg = args[index];
			index++;
			size = strlen(arg);
			for (i = 0; i < size; ++i)
				*str++ = *arg++;
			break;
			}
		default:
			break;
		}
	}
	*str = '\0';

	return str - msg;
}

static void str_replace(char *target, int max_len, char *old, char *str_new)
{
	char *buffer = (char *)malloc(max_len);
	char *beg_of_substr = NULL;

	if (buffer == NULL)
		return;

	beg_of_substr = strstr(target, old);
	if (beg_of_substr == NULL) {
		free(buffer);
		return;
	}

	strncpy_safe(buffer, target, max_len, beg_of_substr - target);
	buffer[beg_of_substr-target] = '\0';

	snprintf(buffer + (beg_of_substr - target), max_len - (beg_of_substr - target), "%s%s", str_new, beg_of_substr + strlen(old));
	target[0] = '\0';
	strncpy_safe(target, buffer, max_len, max_len - 1);
	free(buffer);

	return str_replace(target, max_len, old, str_new);
}

static int get_json_int(json_t *jobj, char *name)
{
	json_t *mjobj = json_object_get(jobj, name);

	if (mjobj == NULL) {
		return INVAILD_IDX;
	}

	int64 id = json_integer_value(mjobj);

	if (-1 == id) {
		return INVAILD_IDX;
	}

	return id;
}

static char *get_json_str(json_t *jobj, char *name)
{
	json_t *jval = json_object_get(jobj, name);

	if (jval == NULL) {
		printf("No such atrribute in the json object: %s", name);
		return NULL;
	}

	char *data = json_string_value(jval);

	return data;
}

static void load_json_str_value(char *dest, int max_len, json_t *jobj, char *name)
{
	char *data = get_json_str(jobj, name);

	if (data == NULL)
		return;
	strncpy_safe(dest, data, max_len, max_len - 1);
}

/*For example: Convert json array [number, string, number] to int array [1,2,1]*/
static void param_type_parse(json_t *jobj, int *types)
{
	int size = json_array_size(jobj);
	int index = 0;
	char *tmp = NULL;

	for (index = 0; index < size; index++) {
		tmp = json_string_value(json_array_get(jobj, index));
		if (tmp == NULL) {
			printf("param_type_parse get json_array_get[%d] failed.\n", index);
			return;
		}
		if (strncmp(TYPE_STR, tmp, strlen(TYPE_STR)) == 0)
			types[index] = TYPE_STR_ID;
		else if (strncmp(TYPE_INT, tmp, strlen(TYPE_INT)) == 0)
			types[index] = TYPE_INT_ID;
		else
			printf("param_type_parse error happened.\n");
	}
}

static struct oem *oem_parse(json_t *jobj)
{
	struct oem *poem = NULL;
	struct oem *poem_header = NULL;
	json_object_t *obj = json_to_object(jobj);
	json_pair_t *pair;

	for (pair = obj->next; pair != NULL; pair = pair->next) {
		if (poem == NULL) {
			poem = (struct oem *)malloc(sizeof(struct oem));
			poem_header = poem;
		} else {
			poem->pnext = (struct oem *)malloc(sizeof(struct oem));
			poem = poem->pnext;
		}
		strncpy_safe(poem->id_str, pair->name, MAX_SEVERITY_SIZE, MAX_SEVERITY_SIZE - 1);
		load_json_str_value(poem->type, MAX_TYPE_SIZE, pair->value, "Type");
	}

	return poem_header;
}

static struct mr_message *msg_parse(json_t *jobj)
{
	struct mr_message	*pmsg			= NULL;
	struct mr_message	*pmsg_header	= NULL;
	struct json_t		*jmsg_body		= NULL;
	struct json_t		*tmp			= NULL;
	int					sn				= 1;
	json_pair_t *pair;
	json_object_t *obj = json_to_object(jobj);

	for (pair = obj->next; pair != NULL; pair = pair->next) {
		if (pmsg == NULL) {
			pmsg = (struct mr_message *)malloc(sizeof(struct mr_message));
			if (pmsg == NULL)
				continue;
            memset(pmsg, 0, sizeof(struct mr_message));
			pmsg_header = pmsg;
		} else {
			pmsg->pnext = (struct mr_message *)malloc(sizeof(struct mr_message));
			if (pmsg->pnext == NULL)
				continue;
            memset(pmsg->pnext, 0, sizeof(struct mr_message));
			pmsg = pmsg->pnext;
		}

		strncpy_safe(pmsg->msg_sn_str, pair->name, MAX_IDENTITY_SIZE, MAX_IDENTITY_SIZE - 1);
		pmsg->msg_sn = sn;
		pmsg->index = get_json_int(pair->value, "_ArgPos");
		load_json_str_value(pmsg->desc,		MAX_SEVERITY_SIZE, pair->value, "Description");
		load_json_str_value(pmsg->msg,			MAX_MESSAGE_SIZE, pair->value, "Message");
		load_json_str_value(pmsg->severity,	MAX_SEVERITY_SIZE, pair->value, "Severity");
		pmsg->num_of_args = (int)json_integer_value(json_object_get(pair->value, "NumberOfArgs"));
		param_type_parse(json_object_get(pair->value, "ParamTypes"), pmsg->types);

		/*
		load_json_str_value(pmsg->resolution,  MAX_LSTR_SIZE, pair->value, "Resolution");
		pmsg->oem_msg = oem_parse(json_object_get(pair->value, "Oem"));
		*/
		sn++;
	}

	return pmsg_header;
}

static void msg_reg_parse(json_t *jobj, struct message_registry *mr)
{
	json_t *oem_json = NULL;
	json_t *mjobj = NULL;
	load_json_str_value(mr->name,		MAX_NAME_SIZE, jobj, "Name");
	load_json_str_value(mr->modified,	MAX_MODIFIED_SIZE, jobj, "Modified");
	load_json_str_value(mr->type,		MAX_TYPE_SIZE, jobj, "Type");
	load_json_str_value(mr->lang,		MAX_LANGUAGE_SIZE, jobj, "Language");
	load_json_str_value(mr->desc,		MAX_DESCRIPTION_SIZE, jobj, "Description");
	load_json_str_value(mr->prefix,	MAX_PREFIX_SIZE, jobj, "RegistryPrefix");
	load_json_str_value(mr->ver,		MAX_VERSION_SIZE, jobj, "Version");
	load_json_str_value(mr->entity,	MAX_ENTITY_SIZE, jobj, "OwningEntity");

	mjobj = json_object_get(jobj, "Messages");
	if (mjobj == NULL){
		printf("fail to get json Message field.\n");
		return;
	}
	mr->msg_header	= msg_parse(mjobj);
	oem_json = json_object_get(jobj, "Oem");;
	if (oem_json == NULL) {
		printf("fail to get json Oem field.\n");
		return;
	}
	mr->oem_data	= oem_parse(oem_json);
}

static struct message_registry *mr_init(char *data)
{
	struct message_registry *mr = (struct message_registry *)malloc(sizeof(struct message_registry));

	json_t *obj = json_parse(data);

	if (!obj) {
		printf("Json parse error...");
		return NULL;
	}

	msg_reg_parse(obj, mr);
	return mr;
}

/*
 Get the message args format from message types(Int array)
 For example: [1,2,1] => ("%d,%s,%d")
*/
int msg_reg_get_args_format(char *msg_args_fmt, int max_len, int msg_sn)
{
	struct mr_message *pmsg		= NULL;
	int		index				= 0;
	va_list args;
	int		ret;
	char	tmp[256]			= {0};

	if (!g_mr)
			return RF_NOT_INIT;

	pmsg = g_mr->msg_header;
	while (pmsg) {
		/*Search all all msg, find msg idx is equal.*/
		if (msg_sn == pmsg->msg_sn) {
			for (index = 0; index < pmsg->num_of_args; index++) {
				if (index != 0)
					strncat(tmp, ",", 1);

				if (pmsg->types[index] == TYPE_STR_ID)
					strncat(tmp, "%s", 2);
				else if (pmsg->types[index] == TYPE_INT_ID)
					strncat(tmp, "%d", 2);
				else
					printf("error message type: %d\n", pmsg->types[index]);
			}

			strncpy_safe(msg_args_fmt, tmp, max_len, max_len - 1);
			return RF_SUCCESS;
		}
		pmsg = pmsg->pnext;
	}

	return RF_NOT_FOUND;
}

static struct message_registry *mr_init_via_file(const char *file_path)
{
	char	*buffer = NULL;
	struct message_registry *mr = NULL;

	if (file_path == NULL)
		buffer = read_file(MR_JSON_FILE_PATH);
	else
		buffer = read_file(file_path);

	if (buffer == NULL)
		return NULL;

	mr = mr_init(buffer);
	
	free(buffer);

	return mr;
}

int msg_reg_get_location_idx(int msg_sn, char *arg_data)
{
	char *args[10];
	int location_idx = INVAILD_IDX;
	int count = get_args_count(arg_data, args);
	int location_offset = -1;
	struct mr_message *pmsg = NULL;

	if (!g_mr)
		return RF_NOT_INIT;

	pmsg = g_mr->msg_header;

	while (pmsg) {
		if (msg_sn == pmsg->msg_sn) {
			location_offset = pmsg->index;
			break;
		}
		pmsg = pmsg->pnext;
	}

	if (count > location_offset && location_offset != -1)
		location_idx = atoi(args[location_offset]);

	return location_idx;
}

int msg_reg_get_msg_id_str(char *identity, int max_len, int msg_sn)
{
	struct mr_message *pmsg = NULL;

	if (!g_mr)
		return RF_NOT_INIT;

	pmsg = g_mr->msg_header;

	while (pmsg) {
		if (msg_sn == pmsg->msg_sn) {
			strncat(identity, g_mr->prefix, max_len);
			strncat(identity, g_mr->ver, max_len);
			strncat(identity, pmsg->msg_sn_str, max_len);
			break;
		}
		pmsg = pmsg->pnext;
	}

	return RF_SUCCESS;
}

int msg_reg_init(const char *path)
{
	if (g_mr == NULL)
		g_mr = mr_init_via_file(path);
	if (g_mr)
		return RF_SUCCESS;
	else
		return RF_ERR;
}

int msg_reg_destory(void)
{
	if (g_mr) {
		free(g_mr);
		g_mr = NULL;
	}

	return RF_SUCCESS;
}

struct message_registry *rf_get_msg_reg()
{
	return g_mr;
}

static void remove_unused_json(json_t* mobj)
{
	json_pair_t *pair;
	json_object_t *obj = json_to_object(mobj);

	for (pair = obj->next; pair != NULL; pair = pair->next) {
		json_object_remove(pair->value, "_ArgPos");
	}
}

extern json_t *rf_get_standard_msg_reg_json()
{
	char	*buffer = NULL;
	json_t  *obj = NULL;
	json_t  *mjobj = NULL;
	buffer = read_file(MR_JSON_FILE_PATH);
	if(buffer ==NULL)
		return NULL;
	obj = json_parse(buffer);
	mjobj = json_object_get(obj, "Messages");
	if (mjobj != NULL) {
		remove_unused_json(mjobj);
	} else {
		printf("rf_get_standard_msg_reg_json Failed to get Json Messages field.\n");
	}

	free(buffer);
	return mjobj;
}

int msg_reg_get_msg_format(char *msg_fmt, int len, int msg_sn)
{
	struct mr_message *pmsg		= NULL;
	int index					= 0;
	va_list args;
	int ret;

	if (!g_mr)
		return RF_NOT_INIT;

	pmsg = g_mr->msg_header;
	while (pmsg) {
		if (msg_sn == pmsg->msg_sn) {
			strncpy_safe(msg_fmt, pmsg->msg, len, len - 1);
			break;
		}
		pmsg = pmsg->pnext;
	}
	if (!pmsg)
		return RF_NOT_FOUND;

	for (index = 0; index < 2; index++)
		str_replace(msg_fmt, len, rf_evt_params_convet_map[index].src, rf_evt_params_convet_map[index].rep);

	return RF_SUCCESS;
}

int msg_reg_get_msg_str(char *msg, int msg_sn, char *arg_data)
{
	if (!g_mr)
		return RF_NOT_INIT;

	int ret = 0;
	char *args[10];
	int count = get_args_count(arg_data, args);
	char msg_fmt[512] = {0};

	ret = msg_reg_get_msg_format(msg_fmt, sizeof(msg_fmt), msg_sn);
	if (ret)
		return ret;
	parse_msg(msg, msg_fmt, args, count);

	return RF_SUCCESS;
}
