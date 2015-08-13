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
 * @file command/compute/json/get_memory_info.cpp
 *
 * @brief JSON command get memory information implementation
 * */

#include "get_memory_info.hpp"
#include "agent-framework/command/compute/get_memory_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::command::compute::json;

GetMemoryInfo::GetMemoryInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                "socket", jsonrpc::JSON_INTEGER,
                nullptr)) { }

void GetMemoryInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetMemoryInfo::Request request{};
        compute::GetMemoryInfo::Response response{};

        request.m_component = params["component"].asString();
        request.m_socket = params["socket"].asUInt();

        command->execute(request, response);

        result["socket"] = response.m_socket;
        result["type"] = response.m_type;
        result["sizeGB"] = response.m_size_gb;
        result["speedMHz"] = response.m_speed_mhz;
        result["voltageVolt"] = response.m_voltage_volt;
        result["dataWidthBits"] = response.m_data_width_bits;
        result["totalWidthBits"] = response.m_total_width_bits;
        result["formFactor"] = response.m_form_factor;
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

void GetMemoryInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetMemoryInfo> g;
