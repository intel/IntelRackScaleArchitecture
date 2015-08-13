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

#include "attribute_dto.hpp"

using namespace psme::core::dto;

AttributeDTO::~AttributeDTO(){}

const Json::Value AttributeDTO::to_json() const {
    Json::Value json_attribute;

    json_attribute["name"] = m_name;
    json_attribute["value"] = m_value;

    return json_attribute;
}

void AttributeDTO::set_value(const std::string& value) {
    m_value["string"] = value;
}

void AttributeDTO::set_value(double value) {
    m_value["number"] = value;
}

void AttributeDTO::set_value(bool value) {
    m_value["boolean"] = value;
}

void AttributeDTO::set_value(const std::vector<std::string>& value) {
    Json::Value array_json{};
    for (const auto& array_item : value) {
        array_json.append(array_item);
    }
    m_value["arrayString"] = std::move(array_json);
}

void AttributeDTO::set_value(const std::vector<double>& value) {
    Json::Value array_json{};
    for (const auto& array_item : value) {
        array_json.append(array_item);
    }
    m_value["arrayNumber"] = std::move(array_json);
}

void AttributeDTO::set_value(const std::vector<bool>& value) {
    Json::Value array_json{};
    for (const auto& array_item : value) {
        array_json.append(array_item);
    }
    m_value["arrayBoolean"] = std::move(array_json);
}

