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

#include "components_dto.hpp"

using namespace psme::core::dto;

ComponentsDTO::~ComponentsDTO(){}

ComponentsDTO::Request::~Request(){}

ComponentsDTO::Response::~Response(){}

const Json::Value ComponentsDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["component"] = get_component_id();

    return json_request;
}

void ComponentsDTO::Response::to_object(const Json::Value& response) {
    if (response.isArray()) {
        for (auto& json_component : response) {
            ComponentDTO::Response component;
            component.set_name(json_component["component"].asString());
            component.set_type(json_component["type"].asString());
            for (auto& json_subcomponent : json_component["components"]){
                ComponentDTO::Response subcomponent;
                subcomponent.to_object(json_subcomponent);
                component.add_subcomponent(subcomponent);
            }
            add_component(component);
        }
    }

}

