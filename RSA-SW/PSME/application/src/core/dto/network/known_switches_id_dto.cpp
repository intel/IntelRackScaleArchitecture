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

#include "known_switches_id_dto.hpp"

using namespace psme::core::dto::network;

KnownSwitchesIdDTO::~KnownSwitchesIdDTO(){}

KnownSwitchesIdDTO::Request::~Request(){}

KnownSwitchesIdDTO::Response::~Response(){}

const Json::Value KnownSwitchesIdDTO::Request::to_json() const {
    Json::Value json_request;
    json_request["component"] = get_component();
    return json_request;
}

void KnownSwitchesIdDTO::Response::to_object(const Json::Value& json) {
    if (json.isObject()) {
        if (json.isMember("switchIdentifiers")) {
            const auto& switches = json["switchIdentifiers"];

            if (switches.isArray()) {
                for (const auto& id_obj : switches) {
                    m_switch_identifiers.push_back(id_obj["Id"].asString());
                }
            }
        }
    }
}
