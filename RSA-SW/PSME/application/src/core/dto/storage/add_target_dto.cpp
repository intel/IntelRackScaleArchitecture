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

#include "add_target_dto.hpp"

using namespace psme::core::dto::storage;

AddTargetDTO::~AddTargetDTO(){}

AddTargetDTO::Request::~Request(){}

AddTargetDTO::Response::~Response(){}

const Json::Value AddTargetDTO::Request::to_json() const {
    Json::Value json_request;

    Json::Value luns(Json::arrayValue);
    for (const auto& lun : m_luns) {
        Json::Value lun_data;
        lun_data["logicalDrive"] = lun.get_drive();
        lun_data["LUN"] = static_cast<unsigned>(lun.get_lun());
        luns.append(std::move(lun_data));
    }

    json_request["targetLUN"] = luns;
    json_request["targetIQN"] = get_target_iqn();
    json_request["initiatorIQN"] = get_initiator_iqn();
    json_request["oem"] = Json::objectValue;

    return json_request;
}

void AddTargetDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        m_target = response["target"].asString();
        m_oem_data.to_object(response["oem"]);
        m_target_address = response["targetAddress"].asString();
        m_target_port = response["targetPort"].asUInt();
    }
}
