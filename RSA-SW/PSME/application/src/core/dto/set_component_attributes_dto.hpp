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
 * @file core/dto/set_component_attributes_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_SET_COMPONENT_ATTRIBUTES_DTO_HPP
#define PSME_CORE_DTO_SET_COMPONENT_ATTRIBUTES_DTO_HPP

#include "core/dto/attribute_dto.hpp"
#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {

/*! SetComponentAttributes request object */
class SetComponentAttributesDTO {
public:
    /*! Copy constructor */
    SetComponentAttributesDTO(const SetComponentAttributesDTO &) = default;

    /*! Assigment constructor */
    SetComponentAttributesDTO& operator=(const SetComponentAttributesDTO &) = default;

    /*! Default constructor */
    SetComponentAttributesDTO() {}

    /*! target information DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::vector<AttributeDTO> m_attributes{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * @brief Get component UUID
         * @return Component UUID
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Set component UUID
         * @param[in] component Component UUID
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * @brief Add attribute
         * @param[in] attribute Attribute
         * */
        void add_attribute(const AttributeDTO& attribute) {
            m_attributes.push_back(attribute);
        }

        /*!
         * @brief Set attributes
         * @param[in] attributes Attributes
         * */
        void set_attributes(const std::vector<AttributeDTO>& attributes) {
            m_attributes = attributes;
        }

        virtual ~Request();
    };

    /*! DTO response */
    class Response : public ResponseDTO {
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        virtual ~Response();
    };

    ~SetComponentAttributesDTO();
};

}
}
}

#endif /* STORAGE_SET_COMPONENT_ATTRIBUTES_DTO_HPP */
