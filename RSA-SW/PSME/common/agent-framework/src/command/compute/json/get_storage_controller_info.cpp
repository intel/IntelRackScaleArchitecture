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
 * */

#include "get_storage_controller_info.hpp"

using namespace agent_framework::command::compute::json;

GetStorageControllerInfo::GetStorageControllerInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "component", jsonrpc::JSON_STRING,
                "controller", jsonrpc::JSON_INTEGER,
                nullptr)) { }

void GetStorageControllerInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetStorageControllerInfo::Request request{};
        compute::GetStorageControllerInfo::Response response{};

        request.m_component = params["component"].asString();
        request.m_controller = params["controller"].asUInt();

        command->execute(request, response);

        result["status"] = response.m_status.to_json();
        result["interface"] = response.m_interface.c_str();
        result["fruInfo"] = response.m_fru_info.to_json();
        result["driveCount"] = response.m_drive_count;
        result["oem"] = response.m_oem_data->to_json();
    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetStorageControllerInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetStorageControllerInfo> g;
