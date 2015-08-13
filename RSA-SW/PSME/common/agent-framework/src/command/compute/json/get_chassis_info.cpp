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
 * @file command/compute/json/get_chassis_info.cpp
 *
 * @brief JSON command get chassis information implementation
 * */

#include "get_chassis_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::command::compute::json;

GetChassisInfo::GetChassisInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "chassis", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetChassisInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetChassisInfo::Request request{};
        compute::GetChassisInfo::Response response{};

        request.m_chassis = params["chassis"].asString();

        command->execute(request, response);

        result["status"] = response.m_status.to_json();
        result["type"] = response.m_type;
        result["size"] = response.m_size;
        result["locationOffset"] = response.m_location_offset;
        result["fruInfo"] = response.m_fru_info.to_json();
        result["oem"] = response.m_oem_data.to_json();
        result["collections"] = response.m_collections.to_json();

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetChassisInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetChassisInfo> g;
