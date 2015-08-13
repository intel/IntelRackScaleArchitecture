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

#include "memory_info_dto.hpp"

using namespace psme::core::dto::compute;

MemoryInfoDTO::~MemoryInfoDTO(){}

MemoryInfoDTO::Request::~Request(){}

MemoryInfoDTO::Response::~Response(){}

const Json::Value MemoryInfoDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["component"] = get_component();
    json_request["socket"] = get_socket();

    return json_request;
}

void MemoryInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_socket(response["socket"].asString());
        set_type(response["type"].asString());
        set_size_gb(response["sizeGB"].asUInt());
        set_speed_mhz(response["speedMHz"].asUInt());
        set_voltage_volt(response["voltageVolt"].asDouble());
        set_data_width_bits(response["dataWidthBits"].asUInt());
        set_total_width_bits(response["totalWidthBits"].asUInt());
        set_form_factor(response["formFactor"].asString());

        m_status.to_object(response["status"]);
        m_fru_info.to_object(response["fruInfo"]);
        m_oem_data.to_object(response["oem"]);
    }
}
