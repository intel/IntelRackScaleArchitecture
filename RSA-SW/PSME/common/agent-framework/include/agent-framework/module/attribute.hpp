/*!
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
 *
 * @file attribute.hpp
 * @brief Attribute
 * */

#ifndef AGENT_FRAMEWORK_MODULE_ATTRIBUTE_HPP
#define AGENT_FRAMEWORK_MODULE_ATTRIBUTE_HPP

#include <string>
#include <json/json.h>

namespace agent_framework {
namespace generic {

/*! Component attribute */
class Attribute {
    std::string m_name{};
    std::string m_string_value{};
    double m_number_value{};
    bool m_boolean_value{};
    std::vector<std::string> m_array_string_value{};
    std::vector<double> m_array_number_value{};
    std::vector<bool> m_array_boolean_value{};
public:
    /*! Default constructor */
    Attribute(const std::string& name = {})
        : m_name(name) {}

    /*! Copy constructor */
    Attribute(const Attribute&) = default;

    /*! Assignment operator */
    Attribute& operator=(const Attribute&) = default;

    /*! Destructor */
    ~Attribute();

    /*!
     * @brief Set attribute name
     * @param[in] name Attribute name
     * */
    void set_name(const std::string& name) {
        m_name = name;
    }

    /*!
     * @brief Get attribute name
     * @return Attribute name
     * */
    const std::string& get_name() const {
        return m_name;
    }

    /*!
     * @brief Set attribute string value
     * @param[in] value Attribute value
     * */
    void set_string_value(const std::string& value) {
        m_string_value = value;
    }

    /*!
     * @brief Get attribute string value
     * @return Attribute value
     * */
    const std::string& get_string_value() const {
        return m_string_value;
    }

    /*!
     * @brief Set attribute number value
     * @param[in] value Attribute value
     * */
    void set_number_value(double value) {
        m_number_value = value;
    }

    /*!
     * @brief Get attribute number value
     * @return Attribute value
     * */
    double get_number_value() const {
        return m_number_value;
    }

    /*!
     * @brief Set attribute boolean value
     * @param[in] value Attribute value
     * */
    void set_boolean_value(bool value) {
        m_boolean_value = value;
    }

    /*!
     * @brief Get attribute boolean value
     * @return Attribute value
     * */
    bool get_boolean_value() const {
        return m_boolean_value;
    }

    /*!
     * @brief Set attribute value
     * @param[in] value Attribute value
     * */
    void set_array_string_value(const std::vector<std::string>& value) {
        m_array_string_value = value;
    }

    /*!
     * @brief Get attribute value
     * @return Attribute value
     * */
    const std::vector<std::string>& get_array_string_value() const {
        return m_array_string_value;
    }

    /*!
     * @brief Set attribute value
     * @param[in] value Attribute value
     * */
    void set_array_number_value(const std::vector<double>& value) {
        m_array_number_value = value;
    }

    /*!
     * @brief Get attribute value
     * @return Attribute value
     * */
    const std::vector<double>& get_array_number_value() const {
        return m_array_number_value;
    }

    /*!
     * @brief Set attribute value
     * @param[in] value Attribute value
     * */
    void set_array_boolean_value(const std::vector<bool>& value) {
        m_array_boolean_value = value;
    }

    /*!
     * @brief Get attribute value
     * @return Attribute value
     * */
    const std::vector<bool>& get_array_boolean_value() const {
        return m_array_boolean_value;
    }

    void from_json(const Json::Value& json);
};

}
}

#endif /* AGENT_FRAMEWORK_MODULE_ATTRIBUTE_HPP */
