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

#include "component_dto.hpp"

using namespace psme::core::dto;

void ComponentDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_name(response["component"].asString());
        set_type(response["type"].asString());
    }
}

void ComponentDTO::Response::add_subcomponent(
        const ComponentDTO::Response& subcomponent) {
    Response::m_components.push_back(subcomponent);
}

ComponentDTO::~ComponentDTO(){}

ComponentDTO::Response::~Response(){}
