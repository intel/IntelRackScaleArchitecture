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

#include "target_info_dto.hpp"

using namespace psme::core::dto::storage;

TargetInfoDTO::~TargetInfoDTO(){}

TargetInfoDTO::Request::~Request(){}

TargetInfoDTO::Response::~Response(){}

const Json::Value TargetInfoDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["target"] = get_target();

    return json_request;
}

void TargetInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        for (const auto& lun : response["targetLUN"]) {
            m_luns.emplace_back(lun["LUN"].asUInt64(),
                                lun["logicalDrive"].asString());
        }
        m_target_iqn = response["targetIQN"].asString();
        m_target_address = response["targetAddress"].asString();
        m_target_port = response["targetPort"].asUInt();
        m_initiator_iqn = response["initiatorIQN"].asString();

        m_status.to_object(response["status"]);
        m_oem_data.to_object(response["oem"]);
    }
}
