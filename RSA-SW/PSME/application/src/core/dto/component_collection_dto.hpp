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
 * @file core/dto/component_collection_dto.hpo
 * @brief GetComponentCollection response transfer obejct declaration.
 *
 * */

#ifndef PSME_CORE_DTO_COMPONENT_COLLECTION_DTO_HPP
#define PSME_CORE_DTO_COMPONENT_COLLECTION_DTO_HPP

#include "request_dto.hpp"
#include "response_dto.hpp"
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

/*! Component collection transfer object */
class ComponentCollectionDTO {
public:
    /*! Copy constructor */
    ComponentCollectionDTO(const ComponentCollectionDTO&) = default;

    /*! Assigment constructor */
    ComponentCollectionDTO& operator=(const ComponentCollectionDTO&) = default;

    /*! Default constructor */
    ComponentCollectionDTO() {}

    /*! Component class */
    class Component {
    public:
        /*! Constructor */
        Component(const std::string& component, const std::string& type)
            : m_component(component), m_type(type) {}

        /*!
         * @brief Set component UUID
         * @param[in] component Component UUID
         */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * @brief Get component UUID
         * @return Component UUID
         */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Set component type
         * @param type Component type
         */
        void set_type(const std::string& type) {
            m_type = type;
        }

        /*!
         * @brief Get component type
         * @return Component type
         */
        const std::string& get_type() const {
            return m_type;
        }

    private:
        std::string m_component{};
        std::string m_type{};
    };

    /*! Component collection DTO request */
    class Request : public RequestDTO {
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        virtual ~Request();
    };

    class Response : public ResponseDTO {
        std::vector<Component> m_components{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO() {}

        /*!
         * Desarializes collection data response object from JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Add component to collection
         * @param component Component object
         */
        void add_component(const Component& component) {
            m_components.emplace_back(component);
        }

        /*!
         * @brief Set components
         * @param components Components array
         */
        void set_components(const std::vector<Component>& components) {
            m_components = components;
        }

        /*!
         * @brief Get components
         * @return Components array
         */
        const std::vector<Component>& get_components() const {
            return m_components;
        }

        virtual ~Response();
    };

    virtual ~ComponentCollectionDTO();
};

}
}
}

#endif /* PSME_CORE_DTO_COMPONENT_COLLECTION_DTO_HPP */

