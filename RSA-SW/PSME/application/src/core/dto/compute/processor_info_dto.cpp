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
 * */

#include "processor_info_dto.hpp"

using namespace psme::core::dto::compute;

ProcessorInfoDTO::~ProcessorInfoDTO(){}

ProcessorInfoDTO::Request::~Request(){}

ProcessorInfoDTO::Response::~Response(){}

ProcessorInfoDTO::Response::CPUID::~CPUID(){}

const Json::Value ProcessorInfoDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["component"] = get_component();
    json_request["socket"] = get_socket();

    return json_request;
}

void ProcessorInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_socket(response["socket"].asString());
        set_processor_type(response["processorType"].asString());
        set_processor_architecture(response["processorArchitecture"].asString());
        set_instruction_set(response["instructionSet"].asString());
        set_manufacturer(response["manufacturer"].asString());
        set_model(response["model"].asString());
        set_max_speed_mhz(response["maxSpeedMHz"].asUInt());
        set_total_cores(response["totalCores"].asUInt());
        set_enabled_cores(response["enabledCores"].asUInt());
        set_total_threads(response["totalThreads"].asUInt());
        set_enabled_threads(response["enabledThreads"].asUInt());

        m_cpuid.to_object(response["cpuid"]);
        m_status.to_object(response["status"]);
    }
}

void ProcessorInfoDTO::Response::CPUID::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_vendor_id(response["vendorId"].asString());
        set_numeric_id(response["numericId"].asUInt());
        set_family(response["family"].asUInt());
        set_model(response["model"].asUInt());
        set_step(response["step"].asUInt());
        set_microcode_info(response["microcodeInfo"].asUInt());
    }
}
