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

#include "component_collection_dto.hpp"

using namespace psme::core::dto;

const Json::Value ComponentCollectionDTO::Request::to_json() const {
    Json::Value json_request;
    return json_request;
}

void ComponentCollectionDTO::Response::to_object(const Json::Value& response) {
    if (response.isArray()) {
        for (const auto& val : response) {
            Component component{val["component"].asString(),
                    val["type"].asString()};
            add_component(std::move(component));
        }
    }
}

ComponentCollectionDTO::~ComponentCollectionDTO(){}

ComponentCollectionDTO::Response::~Response(){}

ComponentCollectionDTO::Request::~Request(){}
