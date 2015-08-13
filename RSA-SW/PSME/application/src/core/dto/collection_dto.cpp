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

#include "collection_dto.hpp"
#include "logger/logger_factory.hpp"

using namespace psme::core::dto;

const Json::Value CollectionDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["component"] = get_component();
    json_request["name"] = get_name();

    return json_request;
}

void CollectionDTO::Response::to_object(const Json::Value& response) {
    if (response.isArray()) {
        for (const auto& val : response) {
            Subcomponent subcomponent{val["subcomponent"].asString()};
            add_subcomponent(std::move(subcomponent));
        }
    }
    else {
        log_error(GET_LOGGER("core"), "Invalid GetCollection response.");
    }
}

CollectionDTO::~CollectionDTO(){}

CollectionDTO::Response::~Response(){}

CollectionDTO::Request::~Request(){}
