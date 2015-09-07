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


#ifndef __LIBREDFISH_MSG_REG_H__
#define __LIBREDFISH_MSG_REG_H__

#include <stdio.h>
#include "libjson/json.h"
 
#define MAX_PARAM_TYPES			20
#define MAX_MSG_COUNT			20

#define M_STR_LEN				256
#define L_STR_LEN				512

#define MAX_ID_STR_SIZE			M_STR_LEN
#define MAX_IDENTITY_SIZE		M_STR_LEN
#define MAX_DESCRIPTION_SIZE	M_STR_LEN
#define MAX_MESSAGE_SIZE		L_STR_LEN
#define MAX_SEVERITY_SIZE		M_STR_LEN
#define MAX_RESOLUTION_SIZE		L_STR_LEN

#define MAX_NAME_SIZE			M_STR_LEN
#define MAX_MODIFIED_SIZE		M_STR_LEN
#define MAX_TYPE_SIZE			M_STR_LEN
#define MAX_LANGUAGE_SIZE		M_STR_LEN
#define MAX_PREFIX_SIZE			M_STR_LEN
#define MAX_VERSION_SIZE		M_STR_LEN
#define MAX_ENTITY_SIZE			M_STR_LEN

#define TYPE_MAX_SIZE			32
#define INVAILD_IDX				0xF

struct oem {
	char		id_str[MAX_ID_STR_SIZE];
	char		type[MAX_TYPE_SIZE];
	struct oem	*pnext;
};

enum {
	STRING,
	NUMBER
};

struct mr_message {
	int			msg_sn;
	int			index;
	char		msg_sn_str[MAX_IDENTITY_SIZE];
	char		desc[MAX_DESCRIPTION_SIZE];
	char		msg[MAX_MESSAGE_SIZE];
	char		severity[MAX_SEVERITY_SIZE];
	int			num_of_args;
	int			types[MAX_PARAM_TYPES];
	char		resolution[MAX_RESOLUTION_SIZE];
	struct oem *oem_msg;

	struct mr_message *pnext;
};

struct message_registry {
	struct oem *oem_data;
	char		name[MAX_NAME_SIZE];
	char		modified[MAX_MODIFIED_SIZE];
	char		type[MAX_TYPE_SIZE];
	char		lang[MAX_LANGUAGE_SIZE];
	char		desc[MAX_DESCRIPTION_SIZE];
	char		prefix[MAX_PREFIX_SIZE];
	char		ver[MAX_VERSION_SIZE];
	char		entity[MAX_ENTITY_SIZE];
	int			msg_cnt;
	struct mr_message *msg_header;
};

struct type_map {
	char src[TYPE_MAX_SIZE];
	char rep[TYPE_MAX_SIZE];
};

enum rf_ret_code {
	RF_SUCCESS,
	RF_NOT_FOUND,
	RF_FORMAT_ERR,
	RF_ERR,
	RF_NOT_INIT
};

/**
 * @brief Initialize redfish message register.
 *
 * @param  path 		the file path of redfish register.
 *
 * @return 0			success.
 * 		   -1			fail.
 */
extern int	msg_reg_init(const char *path);

/**
 * @brief free the message registor data.
 *
 */
extern int	msg_reg_destory(void);

/**
 * @brief get redfish message registor data instance.
 *
 */
extern struct message_registry *rf_get_msg_reg();

/**
 * @brief get standard redfish message registor, remove json key begin with "_"
 *
 */
extern json_t *rf_get_standard_msg_reg_json();

/**
 * @brief get redfish message args format.
 *
 * @param  fmt 				message args format.
 * @param  max_len 			max length of message args format.
 * @param  num 				the num of redfish message.
 */
extern int msg_reg_get_args_format(char *fmt, int max_len, int num);

/**
 * @brief get redfish message format.
 *
 * @param  fmt 				message format.
 * @param  max_len 			max length of message format.
 * @param  num 				the num of redfish message.
 */
extern int msg_reg_get_msg_format(char *fmt, int max_len, int num);

/**
 * @brief get redfish message identity.
 *
 * @param  identity 		message identity.
 * @param  max_len 			max length of message identity.
 * @param  num 				the num of redfish message.
 */
extern int msg_reg_get_msg_id_str(char *identity, int max_len, int num);

/**
 * @brief get redfish message entity.
 *
 * @param  msg 				message entity.
 * @param  max_len 			max length of message entity.
 * @param  num 				the num of redfish message.
 */
extern int msg_reg_get_msg_str(char *msg, int num, char *args);

/**
 * @brief get location index by args(it is decided by num).
 *
 * @param  num 				the num of redfish message.
 * @param  args 			args of message
 */
extern int msg_reg_get_location_idx(int num, char *args);


#endif
