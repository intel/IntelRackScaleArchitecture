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

#include "remote_switch_info_dto.hpp"

using namespace psme::core::dto::network;

RemoteSwitchInfoDTO::~RemoteSwitchInfoDTO(){}

RemoteSwitchInfoDTO::Request::~Request(){}

RemoteSwitchInfoDTO::Response::~Response(){}

const Json::Value RemoteSwitchInfoDTO::Request::to_json() const {
    Json::Value json_request;
    json_request["component"] = get_component();
    json_request["switchIdentifier"] = get_switch_identifier();
    return json_request;
}

void RemoteSwitchInfoDTO::Response::to_object(const Json::Value& json) {
    if (json.isObject()) {
        m_status.to_object(json["status"]);
        set_mac_address(json["macAddress"].asString());

        for (const auto& entry: json["nextHop"]) {
            m_next_hop.to_object(entry);
        }
        m_oem.to_object(json["oem"]);
    }
}
