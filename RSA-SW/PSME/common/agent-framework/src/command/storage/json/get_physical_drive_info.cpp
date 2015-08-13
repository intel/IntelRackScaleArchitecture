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
 * @file command/storage/json/get_physical_drive_info.cpp
 *
 * @brief JSON command getPhysicalDriveInfo
 * */

#include "get_physical_drive_info.hpp"

using namespace agent_framework::command::storage::json;

GetPhysicalDriveInfo::GetPhysicalDriveInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "drive", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetPhysicalDriveInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        storage::GetPhysicalDriveInfo::Request request{};
        storage::GetPhysicalDriveInfo::Response response{};

        request.m_drive = params["drive"].asString();

        command->execute(request, response);

        result["interface"] = response.m_interface;
        result["capacityGB"] = response.m_capacity_gb;
        result["type"] = response.m_type;
        result["rpm"] = response.m_rpm;
        result["fruInfo"] = response.m_fru_info.to_json();
        result["status"] = response.m_status.to_json();
        result["oem"] = response.m_oem->to_json();
    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetPhysicalDriveInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetPhysicalDriveInfo> g;
