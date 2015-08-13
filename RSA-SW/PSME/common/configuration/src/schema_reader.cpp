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
 * @file schema_reader.hpp
 *
 * @brief SchemaReader implementation
 * */

#include "configuration/schema_reader.hpp"
#include "configuration/schema_validator.hpp"
#include "configuration/schema_property.hpp"
#include "configuration/json_path.hpp"
#include "configuration/validators/validators.hpp"
#include "json/json.hpp"

using namespace configuration;

class SchemaReader::Impl {
    JsonPath m_json_path{};
public:
    using ValidatorJsonSPtr = std::shared_ptr<Validator<json::Value>>;

    bool load_schema(const json::Value& schema, SchemaValidator& validator) {
        for (auto it = schema.cbegin(); it != schema.cend(); it++) {
            m_json_path.push_key(it.key());

            if (is_validator_object(*it)) {
                process_json_property(m_json_path.get_path(), *it, validator);
                m_json_path.pop_key();
                continue;
            }

            load_schema(*it, validator);

            m_json_path.pop_key();
        }
        return true;
    }

    void process_json_property(const std::string& name,
                                                const json::Value& value,
                                                SchemaValidator& validator) {
        if (is_validator_object(value)) {
            auto property = make_property(name);
            make_validators(property, value);

            validator.add_property(property);
        }
    }

    SchemaProperty make_property(const std::string& name) {
        return SchemaProperty{name};
    }

    void make_validators(SchemaProperty& property,
                                                    const json::Value& item) {
        if (auto validator = create_max_validator(item)) {
            property.add_validator(validator);
        }

        if (auto validator = create_min_validator(item)) {
            property.add_validator(validator);
        }

        if (auto validator = create_type_validator(item)) {
            property.add_validator(validator);
        }

        if (auto validator = create_address_validator(item)) {
            property.add_validator(validator);
        }

        if (auto validator = create_url_validator(item)) {
            property.add_validator(validator);
        }

        if (auto validator = create_anyof_validator(item)) {
            property.add_validator(validator);
        }
    }

    ValidatorJsonSPtr create_max_validator(const json::Value& value) {
        if (value["max"].is_number()) {
            return std::make_shared<MaxValidator>(value["max"].as_int());
        }
        return nullptr;
    }

    ValidatorJsonSPtr create_min_validator(const json::Value& value) {
        if (value["min"].is_number()) {
            return std::make_shared<MinValidator>(value["min"].as_int());
        }
        return nullptr;
    }

    ValidatorJsonSPtr create_type_validator(const json::Value& value) {
        if (value["type"].is_string()) {
            return std::make_shared<TypeValidator>(value["type"].as_string());
        }
        return nullptr;
    }

    ValidatorJsonSPtr create_address_validator(const json::Value& value) {
        if (value["address"].is_string()) {
            return std::make_shared<AddressValidator>(
                                                value["address"].as_string());
        }
        return nullptr;
    }

    ValidatorJsonSPtr create_url_validator(const json::Value& value) {
        if (value["url"].is_string()) {
            return std::make_shared<UrlValidator>(
                                                value["url"].as_string());
        }
        return nullptr;
    }

    ValidatorJsonSPtr create_anyof_validator(const json::Value& value) {
        if (value["anyof"].is_array() && value.is_member("type")) {
            const auto type = value["type"].as_string();
            if ("string" == type) {
                return create_anyof_string(value);
            }

            if ("int" == type) {
                return create_anyof_int(value);
            }
        }
        return nullptr;
    }

    ValidatorJsonSPtr create_anyof_string(const json::Value& value) {
        auto validator = std::make_shared<AnyOfValidator<std::string>>();
        for (const auto& val : value["anyof"]) {
            if (val.is_string()) {
                validator->add_constraint(val.as_string());
            }
        }
        return validator;
    }

    ValidatorJsonSPtr create_anyof_int(const json::Value& value) {
        auto validator = std::make_shared<AnyOfValidator<int>>();
        for (const auto& val : value["anyof"]) {
            if (val.is_number()) {
                validator->add_constraint(val.as_int());
            }
        }
        return validator;
    }


    bool is_validator_object(const json::Value& value) {
         return value.is_object() && value.is_member("validator") &&
             (value.is_member("max") ||
              value.is_member("min") ||
              value.is_member("type") ||
              value.is_member("addrV") ||
              value.is_member("url") ||
              value.is_member("anyof"));
    }
};

SchemaReader::SchemaReader() :
    m_impl{new SchemaReader::Impl} {}

SchemaReader::~SchemaReader() {}

bool SchemaReader::load_schema(const json::Value& schema,
                                                SchemaValidator& validator) {
    return m_impl->load_schema(schema, validator);
}
