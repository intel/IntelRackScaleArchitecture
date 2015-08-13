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
 * @file core/dto/attribute_dto.hpp
 * @brief Attribute transfer object declaration.
 *
 * */
#ifndef PSME_CORE_DTO_ATTRIBUTE_DTO_HPP
#define PSME_CORE_DTO_ATTRIBUTE_DTO_HPP

#include "request_dto.hpp"

#include <string>

namespace Json {
class Value;
}

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Dto namespace */
namespace dto {

/*! Attribute transfer object */
class AttributeDTO {
    Json::Value m_value{};
    std::string m_name{};
public:
    /*! Copy constructor */
    AttributeDTO(const AttributeDTO &) = default;

    /*! Assigment constructor */
    AttributeDTO& operator=(const AttributeDTO &) = default;

    /*! Default constructor */
    AttributeDTO() {}

    /*! Serializes object to JSON
     *
     *  @return Request object serialized to JSON.
     * */
    const Json::Value to_json() const;

    /*!
     * @brief Set attribute name
     * @param[in] name Attribute name
     * */
    void set_name(const std::string& name) {
        m_name = name;
    }

    /*!
     * @brief Set attribute value
     * @param[in] value Attribute value
     * */
    void set_value(const std::string& value);

    /*!
     * @brief Set attribute value
     * @param[in] value Attribute value
     * */
    void set_value(double value);

    /*!
     * @brief Set attribute value
     * @param[in] value Attribute value
     * */
    void set_value(bool value);

    /*!
     * @brief Set attribute value
     * @param[in] value Attribute value
     * */
    void set_value(const std::vector<std::string>& value);

    /*!
     * @brief Set attribute value
     * @param[in] value Attribute value
     * */
    void set_value(const std::vector<double>& value);

    /*!
     * @brief Set attribute value
     * @param[in] value Attribute value
     * */
    void set_value(const std::vector<bool>& value);

    virtual ~AttributeDTO();
};

}
}
}

#endif /* PSME_CORE_DTO_ATTRIBUTE_DTO_HPP */

