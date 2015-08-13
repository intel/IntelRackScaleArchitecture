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
 * @file schema_validator.cpp
 *
 * @brief SchemaValidator implementation
 * */

#include "configuration/schema_validator.hpp"
#include "configuration/schema_property.hpp"
#include "configuration/json_path.hpp"
#include "configuration/utils.hpp"
#include "json/json.hpp"

using namespace configuration;

class SchemaValidator::Impl {
public:
    void add_property(const SchemaProperty& property) {
        m_properties.push_back(property);
    }

    void validate(const json::Value& json, SchemaErrors& errors) {
        for (auto it = json.cbegin(); it != json.cend(); it++) {
            m_json_path.push_key(it.key());

            // if there is no validator for module check global property
            if (!check_property(m_json_path.get_path(), *it, errors)) {
                check_property(it.key(), *it, errors);
            }
            validate(*it, errors);

            m_json_path.pop_key();
        }
    }

    bool check_property(const std::string& name, const json::Value& value,
                                                        SchemaErrors& errors) {
        for (auto& property : m_properties) {
            if (property.get_name() == name) {
                auto error = property.validate(value);
                if (error.count()) {
                    error.set_path(m_json_path.get_path());
                    error.set_value(json_value_to_string(value));
                    errors.add_error(error);
                }
                return true;
            }
        }
        return false;
    }

    using property_t = std::vector<SchemaProperty>;
    property_t m_properties{};

    JsonPath m_json_path{};
};

SchemaValidator::SchemaValidator() :
    m_impl{new SchemaValidator::Impl} {}

SchemaValidator::~SchemaValidator() {}

void SchemaValidator::add_property(const SchemaProperty& property) {
    m_impl->add_property(property);
}

void SchemaValidator::validate(const json::Value& json, SchemaErrors& errors) {
    m_impl->validate(json, errors);
}

