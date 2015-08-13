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
 * @file schema_property.hpp
 *
 * @brief SchemaProperty implementation
 * */

#include "configuration/schema_property.hpp"
#include "configuration/schema_errors.hpp"
#include "configuration/validators/validator.hpp"
#include "json/json.hpp"

using namespace configuration;

SchemaProperty::SchemaProperty(const std::string& name) : m_name{name} {}

SchemaErrors::Error SchemaProperty::validate(const json::Value& value) {
    SchemaErrors::Error error{m_name};
    for (const auto& validator : m_validators) {
        if (!validator->is_valid(value)) {
            error.add_error(validator->get_error());
        }
    }
    return error;
}

void SchemaProperty::add_validator(ValidatorJsonSPtr validator) {
    m_validators.push_back(validator);
}

