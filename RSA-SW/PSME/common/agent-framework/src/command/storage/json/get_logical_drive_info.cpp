/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section DESCRIPTION
 *
 * @file command/storage/json/get_logical_drive_info.cpp
 *
 * @brief JSON command getLogicalDriveInfo
 * */

#include "get_logical_drive_info.hpp"

using namespace agent_framework::command::storage::json;

GetLogicalDriveInfo::GetLogicalDriveInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "drive", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetLogicalDriveInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        storage::GetLogicalDriveInfo::Request request{};
        storage::GetLogicalDriveInfo::Response response{};

        request.m_drive = params["drive"].asString();

        command->execute(request, response);

        result["capacityGB"] = response.m_capacity_gb;
        result["type"] = response.m_type;
        result["status"] = response.m_status.to_json();
        result["mode"] = response.m_mode;
        result["image"] = response.m_image;
        result["bootable"] = response.m_bootable;
        result["protected"] = response.m_protected;
        result["snapshot"] = response.m_snapshot;
        result["collections"] = response.m_collection.to_json();
        result["oem"] = response.m_oem->to_json();
        result["master"] = response.m_master;
    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Drive not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetLogicalDriveInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetLogicalDriveInfo> g;
