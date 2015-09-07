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


#ifndef __LIBCFGPARSER_RMM_CFG_H__
#define __LIBCFGPARSER_RMM_CFG_H__

#include "platform.h"

#define RMM_LOG_MODULE_NAME_LEN 	128
#define MAX_LOG_MODULES_CNT 		10
#define RMM_TTY_INFO_LEN			128 
#define MAX_TTY_INFOS_CNT   		8
#define MAX_VM_PASSWORD_LEN			16

struct rmm_log_module {
	char name[RMM_LOG_MODULE_NAME_LEN];
	int level;
	int max_lines;
};

enum {
	ASSETD_PORT = 0,
	RESTD_PORT,
	MEMDBD_PORT,
	IPMIMODULE_PORT,
	REDFISHD_PORT,
	SNMPAGENTD_PORT,
	SENSORD_PORT,
	UPGRADED_PORT,
	LOGD_PORT,
	DBSYNCD_PORT,
	RMMKEEPERD_PORT,
	SYNCDB_VER_PORT,
	KEEPERDB_PORT,
	SNAPSHOT_SYNC_PORT,
	IPMIRMCPCLIENT_PORT,
	IPMIIPMB_PORT,
	IPMII2CD_PORT,
	IPMI_LISTEN_PORT,
	IPMICOOLINGCTRL_PORT,
	IPMIPOWERMGMT_PORT,
	IPMIASSETMODULE_PORT,
	IPMISENSORD_PORT,
	IPMITEST_PORT,
	IPMIUARTTGTCFG_PORT,
	IPMIWEBSOCKET_PORT,
	IPMIJSONRPC_SERVER_PORT,
	IPMIJSONRPC_IPMID_PORT,
	IPMIJSONRPC_SNMP_SUBAGENT_PORT,
	ASSETMGR_JSONRPC_PORT,
	REGISTERD_PORT,
	REGISTERD_JSONRPC_PORT,
	ASSET_MODULE_PORT,
};

/**
 * @brief rmm config get tty info
 *
 * @param  infos 			information of tty
 * @param  count 			tty count in config file.
 */
int rmm_cfg_get_tty_infos(char *infos, int *count);

/**
 * @brief get rmm log modules
 *
 * @param  modules 			log modules.
 * @param  count 			The count of log modules.
 */
int rmm_cfg_get_log_modules(struct rmm_log_module *modules, int *count);

int rmm_cfg_get_port(int port_attr);

int rmm_cfg_get_baudrate();
unsigned char rmm_cfg_get_databits(void);
unsigned char rmm_cfg_get_stopbits(void);
unsigned char rmm_cfg_get_parity(void);
unsigned char rmm_cfg_get_flow_control(void);

int rmm_cfg_get_rmcp_username(char *username, int max_len);
int rmm_cfg_get_rmcp_password(char *password, int max_len);

int rmm_cfg_get_vm_root_password(char *password, int max_len, int vm_idx);
int rmm_cfg_get_rest_prefix(char *prefix, int max_len);

int rmm_cfg_get_platform(char *platform);
int is_platform_bdcr(void);

#endif
