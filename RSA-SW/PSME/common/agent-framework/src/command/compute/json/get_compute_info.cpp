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
 * @file command/compute/json/get_compute_info.cpp
 *
 * @brief JSON command get compute information implementation
 * */

#include "get_compute_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::command::compute::json;

GetComputeInfo::GetComputeInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "compute", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetComputeInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetComputeInfo::Request request{};
        compute::GetComputeInfo::Response response{};

        request.m_compute = params["compute"].asString();

        command->execute(request, response);

        result["status"] = response.m_status.to_json();
        result["fruInfo"] = response.m_fru_info.to_json();
        result["powerZone"] = response.m_power_zone;
        result["thermalZone"] = response.m_thermal_zone;
        result["slot"] = response.m_slot;
        result["oem"] = response.m_oem.to_json();

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetComputeInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetComputeInfo> g;
