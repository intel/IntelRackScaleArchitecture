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
 * @file command/storage/json/get_drive_info.cpp
 *
 * @brief JSON command getDriveInfo
 * */

#include "get_drive_info.hpp"

using namespace agent_framework::command::compute::json;

GetDriveInfo::GetDriveInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "component", jsonrpc::JSON_STRING,
                "controller", jsonrpc::JSON_INTEGER,
                "drive", jsonrpc::JSON_INTEGER,
                nullptr)) { }

void GetDriveInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetDriveInfo::Request request{};
        compute::GetDriveInfo::Response response{};

        request.m_drive = params["drive"].asUInt();
        request.m_controller = params["controller"].asUInt();
        request.m_component = params["component"].asString();

        command->execute(request, response);

        result["interface"] = response.m_interface;
        result["type"] = response.m_type;
        result["capacityGB"] = response.m_capacity_gb;
        result["rpm"] = response.m_rpm;
        result["status"] = response.m_status.to_json();
        result["fruInfo"] = response.m_fru_info.to_json();
        result["oem"] = response.m_oem->to_json();
    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetDriveInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetDriveInfo> g;
