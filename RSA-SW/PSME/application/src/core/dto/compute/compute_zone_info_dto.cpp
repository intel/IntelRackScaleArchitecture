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

#include "compute_zone_info_dto.hpp"

using namespace psme::core::dto::compute;

ComputeZoneInfoDTO::~ComputeZoneInfoDTO(){}

ComputeZoneInfoDTO::Request::~Request(){}

ComputeZoneInfoDTO::Response::~Response(){}

const Json::Value ComputeZoneInfoDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["zone"] = get_zone();

    return json_request;
}

void ComputeZoneInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        m_status.to_object(response["status"]);
        m_collections.to_object(response["collections"]);
        m_oem_data.to_object(response["oem"]);
    }
}
