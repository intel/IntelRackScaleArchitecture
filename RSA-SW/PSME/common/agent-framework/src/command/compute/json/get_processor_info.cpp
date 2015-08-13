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
 * @file command/compute/json/get_processor_info.cpp
 *
 * @brief JSON command get processor information implementation
 * */

#include "get_processor_info.hpp"

using namespace agent_framework::command::compute::json;

GetProcessorInfo::GetProcessorInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                "socket", jsonrpc::JSON_INTEGER,
                nullptr)) { }

void GetProcessorInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetProcessorInfo::Request request{};
        compute::GetProcessorInfo::Response response{};

        request.m_component = params["component"].asString();
        request.m_socket = params["socket"].asUInt();

        command->execute(request, response);

        result["socket"] = response.m_socket;
        result["processorType"] = response.m_processor_type;
        result["processorArchitecture"] = response.m_processor_architecture;
        result["instructionSet"] = response.m_instruction_set;
        result["manufacturer"] = response.m_manufacturer;
        result["model"] = response.m_model;
        result["maxSpeedMHz"] = response.m_max_speed_mhz;
        result["totalCores"] = response.m_total_cores;
        result["enabledCores"] = response.m_enabled_cores;
        result["totalThreads"] = response.m_total_threads;
        result["enabledThreads"] = response.m_enabled_threads;

        result["cpuid"]["vendorId"] = response.m_cpuid.m_vendor_id;
        result["cpuid"]["numericId"] = response.m_cpuid.m_numeric_id;
        result["cpuid"]["family"] = response.m_cpuid.m_family;
        result["cpuid"]["model"] = response.m_cpuid.m_model;
        result["cpuid"]["step"] = response.m_cpuid.m_step;
        result["cpuid"]["microcodeInfo"] = response.m_cpuid.m_microcode_info;

        result["status"] = response.m_status.to_json();
        result["oem"] = response.m_oem->to_json();


    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetProcessorInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetProcessorInfo> g;
