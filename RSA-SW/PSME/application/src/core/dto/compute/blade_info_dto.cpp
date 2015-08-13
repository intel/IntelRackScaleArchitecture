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

#include "blade_info_dto.hpp"

using namespace psme::core::dto::compute;

BladeInfoDTO::~BladeInfoDTO(){}

BladeInfoDTO::Request::~Request(){}

BladeInfoDTO::Response::~Response(){}

const Json::Value BladeInfoDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["component"] = get_component();

    return json_request;
}

namespace {
    BladeInfoDTO::Response::BootSupportedVec
    json_to_boot_supported(const Json::Value& json) {
        BladeInfoDTO::Response::BootSupportedVec boot_supported;
        for (const auto& boot_json : json) {
            boot_supported.emplace_back(boot_json.asString());
        }
        return boot_supported;
    }
}

void BladeInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_bios_version(response["biosVersion"].asString());
        set_processor_count(response["processorCount"].asUInt());
        set_dimm_count(response["dimmCount"].asUInt());
        set_controller_count(response["controllerCount"].asUInt());
        set_nic_count(response["nicCount"].asUInt());
        set_power_state(response["powerState"].asString());
        set_slot(response["slot"].asUInt());
        set_boot_override(response["bootOverride"].asString());
        set_boot_override_target(response["bootOverrideTarget"].asString());
        set_boot_override_supported(
                    json_to_boot_supported(response["bootOverrideSupported"]));
        m_status.to_object(response["status"]);
        m_fru_info.to_object(response["fruInfo"]);
        m_oem_data.to_object(response["oem"]);
    }
}
