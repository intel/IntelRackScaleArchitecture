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


#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "libjson/json.h"
#include "libutils/types.h"
#include "../rest.h"
#include "libutils/upgrade.h"
#include "libutils/sock.h"
#include "libredfish/rf_event.h"
#include "libredfish/mr_def.h"
#include "libutils/rmm_json.h"
#include "librmmcfg/rmm_cfg.h"
#include "libutils/base64.h"

#define MAX_READ_BASE64_SIZE	(64)
#define UPGRADE_FILE_TARBALL	"/tmp/rsa_rmm_upgrade.tar.gz"
#define UPGRADE_DEB_FILE		"rsa-rmm-meta"

#define FIRMWARE_FILE_PATH		TFTP_SERVER_PATH
#define FIRMWARE_FILE_NAME		"F06e.bin"
#define  INVAILD_IDX 0xF


extern void register_rack_handler(void);
extern void register_mbp_handler(void);
extern void register_drawer_handler(void);
extern void register_pzone_handler(void);
extern void register_psu_handler(void);
extern void register_tzone_handler(void);
extern void register_fan_handler(void);

extern void put_prepare_int(const json_t *elem, uint32 *output, const int8 *name);
extern void put_prepare_str(const json_t *req, uint8 *output, int32 len, const int8 *name);
extern int32 prepare_get_action(const avail_action_t *av_action, json_t *action, int32 index);
extern int32 get_asset_idx(const struct rest_uri_param *param, const int8 *name, int32 type);

extern void add_loc_info(json_t *loc, int32 input, const int8 *name);
extern memdb_integer get_asset_node_id(const struct rest_uri_param *param, const int8 *name);
extern int32 pack_basic_element_info(const base_element_t *be, json_t *result);
extern int32 pack_fru_info(const fru_info_t *fru_info, json_t *fru);
extern int32 get_rf_base_info(rf_base_t *base);
extern int32 pack_rf_base_json(json_t *result, const rf_base_t *base);
extern json_t *process_listener(json_t *req, listener_dest_t *listener, int32 mask, int32 node_type);
extern json_t *process_snmp_listener(json_t *req);
extern void pack_rf_evt_links(json_t *result, rf_link_t *link);
extern memdb_integer get_uniq_node_by_type(int32 type);
extern int32 duplicate_check_string(int8 *resource, int8 *string, int32 count, int32 max_len);
extern void get_event_type_by_nodeid(int32 nodeid, int8 *data, int32 data_len);
extern json_t *get_json_listener(listener_dest_info_t *header);
extern int32 subscribe_event(listener_dest_t *listener, int8 *evt_str, int32 type);
extern int32 unsubscribe_event(listener_dest_t *listener, int8 *evt_str, int32 type);
extern void get_evt_links(rf_link_t *links, int32 type, int32 reg, const int8 *fmt, ...);
extern void update_href_host(uint8 *href, int32 len, int8 *host);

#endif
