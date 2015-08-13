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
 * @brief SchemaProperty interface
 * */

#ifndef CONFIGURATION_SCHEMA_PROPERTY_HPP
#define CONFIGURATION_SCHEMA_PROPERTY_HPP

#include "schema_errors.hpp"
#include <vector>
#include <memory>

namespace json { class Value; }

namespace configuration {

template <typename T>
class Validator;

/*! Class store collection of validator assigned to json property */
class SchemaProperty {
public:
    using ValidatorJsonSPtr = std::shared_ptr<Validator<json::Value>>;

    /*!
     * @brief Construct SchemaProperty object for given name
     * @param name Property name
     */
    explicit SchemaProperty(const std::string& name);

    /*!
     * @brief Add validator to property validators list
     * @param validator Validator pointer
     */
    void add_validator(ValidatorJsonSPtr validator);

    /*!
     * @brief Validate given JSON object
     * @param value JSON object
     * @return Error object
     */
    SchemaErrors::Error validate(const json::Value& value);

    /*!
     * @brief Return number of validator assigned to property
     * @return Number of validators
     */
    std::size_t validator_count() const { return m_validators.size(); }

    /*!
     * @brief Get property name
     * @return Property name
     */
    const std::string& get_name() const { return m_name; }

private:
    /*! validator list type */
    using validators_t = std::vector<ValidatorJsonSPtr>;

    std::string m_name;
    validators_t m_validators{};
};
}

#endif /* CONFIGURATION_SCHEMA_PROPERTY_HPP */
