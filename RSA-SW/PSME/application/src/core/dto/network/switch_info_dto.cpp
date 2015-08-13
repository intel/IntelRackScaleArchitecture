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

#include "switch_info_dto.hpp"

using namespace psme::core::dto::network;

SwitchInfoDTO::~SwitchInfoDTO(){}

SwitchInfoDTO::Request::~Request(){}

SwitchInfoDTO::Response::~Response(){}

const Json::Value SwitchInfoDTO::Request::to_json() const {
    Json::Value json_request;
    json_request["component"] = get_component();
    return json_request;
}

void SwitchInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_technology(response["technology"].asString());
        set_mac_address(response["macAddress"].asString());

        m_fru_info.to_object(response["fruInfo"]);
        m_status.to_object(response["status"]);
        m_oem.to_object(response["oem"]);
    }
}
