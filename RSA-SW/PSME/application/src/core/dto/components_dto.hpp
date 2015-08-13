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
 * @file core/dto/request/components_dto.hpp
 * @brief Request transfer obejct declaration.
 * */

#ifndef COMPONENTS_DTO_HPP
#define COMPONENTS_DTO_HPP

#include "request_dto.hpp"
#include "response_dto.hpp"
#include "component_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {

/*! Components request object */
class ComponentsDTO {
public:
    /*! Copy constructor */
    ComponentsDTO(const ComponentsDTO &) = default;

    /*! Assigment constructor */
    ComponentsDTO& operator=(const ComponentsDTO &) = default;

    /*! Default constructor */
    ComponentsDTO() {}

    /*! Components DTO request */
    class Request : public RequestDTO {
        std::string m_component_id{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*!
         * Sets component id
         *
         * @param component_id Component id
         * */
        void set_component_id(const std::string& component_id) {
            m_component_id = component_id;
        }

        /*!
         * Gets component id
         *
         * @return Component id
         * */
        const std::string& get_component_id() const {
            return m_component_id;
        }
        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        virtual ~Request();
    };

    /*! Components DTO response */
    class Response : public ResponseDTO {
        std::vector<ComponentDTO::Response> m_components{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes components response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * Adds new component
         *
         * @param component New component
         * */
        void add_component(const ComponentDTO::Response& component) {
            m_components.push_back(component);
        }

        /*!
         * Gets vector of components
         *
         * @return List of components.
         * */
        const std::vector<ComponentDTO::Response>& get_components()
            const { return m_components; }

        virtual ~Response();
    };

    ~ComponentsDTO();
};

}
}
}

#endif /* COMPONENTS_DTO_HPP */
