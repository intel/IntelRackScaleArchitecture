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

#include "chassis_info_dto.hpp"

using namespace psme::core::dto::compute;

ChassisInfoDTO::~ChassisInfoDTO(){}

ChassisInfoDTO::Request::~Request(){}

ChassisInfoDTO::Response::~Response(){}

const Json::Value ChassisInfoDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["chassis"] = get_chassis();

    return json_request;
}

void ChassisInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_type(response["type"].asString());
        set_location_offset(response["locationOffset"].asUInt());
        set_size(response["size"].asUInt());
        m_status.to_object(response["status"]);
        m_fru_info.to_object(response["fruInfo"]);
        m_collections.to_object(response["collections"]);
        m_oem_data.to_object(response["oem"]);
    }
}
