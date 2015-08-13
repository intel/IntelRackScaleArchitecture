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
 * @file core/dto/collection_dto.hpo
 * @brief GetCollection response transfer obejct declaration.
 *
 * */

#ifndef PSME_CORE_DTO_COLLECTION_DTO_HPP
#define PSME_CORE_DTO_COLLECTION_DTO_HPP

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

/*! Collection transfer object */
class CollectionDTO {
public:
    /*! Copy constructor */
    CollectionDTO(const CollectionDTO&) = default;

    /*! Assigment constructor */
    CollectionDTO& operator=(const CollectionDTO&) = default;

    /*! Default constructor */
    CollectionDTO() {}

    /*! Subcomponent class */
    class Subcomponent {
    public:
        /*! Constructor */
        Subcomponent(const std::string& subcomponent)
            : m_subcomponent(subcomponent) {}

        /*!
         * @brief Set subcomponent UUID
         * @param[in] subcomponent Subcomponent UUID
         */
        void set_subcomponent(const std::string& subcomponent) {
            m_subcomponent = subcomponent;
        }

        /*!
         * @brief Get subcomponent UUID
         * @return Subcomponent UUID
         */
        const std::string& get_subcomponent() const {
            return m_subcomponent;
        }

    private:
        std::string m_subcomponent{};
    };

    /*! Collection DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::string m_name{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*!
         * @brief Sets component UUID
         * @param[in] component Component UUID
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * @brief Gets component UUID
         * @return Component UUID
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Sets collection name
         * @param[in] name Collection name
         * */
        void set_name(const std::string& name) {
            m_name = name;
        }

        /*!
         * @brief Gets collection name
         * @return Collection name
         * */
        const std::string& get_name() const {
            return m_name;
        }

        /*!
         * @brief Serializes object to JSON
         * @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        virtual ~Request();
    };

    class Response : public ResponseDTO {
        std::vector<Subcomponent> m_subcomponents{};
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
         * @brief Add subcomponent to collection
         * @param subcomponent Subcomponent object
         */
        void add_subcomponent(const Subcomponent& subcomponent) {
            m_subcomponents.emplace_back(subcomponent);
        }

        /*!
         * @brief Set subcomponents
         * @param subcomponents Subcomponents array
         */
        void set_subcomponents(const std::vector<Subcomponent>& subcomponents) {
            m_subcomponents = subcomponents;
        }

        /*!
         * @brief Get subcomponents
         * @return Subcomponents array
         */
        const std::vector<Subcomponent>& get_subcomponents() const {
            return m_subcomponents;
        }

        virtual ~Response();
    };

    virtual ~CollectionDTO();
};

}
}
}

#endif /* PSME_CORE_DTO_COLLECTION_DTO_HPP */

