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

#include "agent-framework/module/attribute.hpp"

using namespace agent_framework::generic;

Attribute::~Attribute(){}

void Attribute::from_json(const Json::Value& json) {
    m_name = json["name"].asString();

    const auto& value_json = json["value"];

    if (!value_json.isObject()) {
        return;
    }

    if (value_json["string"].isString()) {
        m_string_value = value_json["string"].asString();
    }
    else if (value_json["number"].isDouble()) {
        m_number_value = value_json["number"].asDouble();
    }
    else if (value_json["boolean"].isBool()) {
        m_boolean_value = value_json["boolean"].asBool();
    }
    else if (value_json["arrayString"].isArray()) {
        std::vector<std::string> array{};
        for (const auto& array_element : value_json["arrayString"]) {
            array.push_back(array_element.asString());
        }
        m_array_string_value = std::move(array);
    }
    else if (value_json["arrayNumber"].isArray()) {
        std::vector<double> array{};
        for (const auto& array_element : value_json["arrayNumber"]) {
            array.push_back(array_element.asDouble());
        }
        m_array_number_value = std::move(array);
    }
    else if (value_json["arrayBoolean"].isArray()) {
        std::vector<bool> array{};
        for (const auto& array_element : value_json["arrayBoolean"]) {
            array.push_back(array_element.asBool());
        }
        m_array_boolean_value = std::move(array);
    }
}
