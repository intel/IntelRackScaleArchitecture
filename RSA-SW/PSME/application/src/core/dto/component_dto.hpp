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
 * @file core/dto/response/component_dto.hpp
 * @brief component response transfer obejct declaration.
 *
 * */

#ifndef COMPONENT_DTO
#define COMPONENT_DTO

#include "response_dto.hpp"

#include <json/json.h>

#include <string>
#include <vector>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Dto namespace */
namespace dto {

/*! Component transfer object */
class ComponentDTO {
    std::string m_type{};
    std::string m_name{};
    std::vector<ComponentDTO> m_components{};

public:
    /*! Copy constructor */
    ComponentDTO(const ComponentDTO &) = default;

    /*! Assigment constructor */
    ComponentDTO& operator=(const ComponentDTO &) = default;

    /*! Default constructor */
    ComponentDTO() {}

    class Response : ResponseDTO {
        std::string m_type{};
        std::string m_name{};
        std::vector<ComponentDTO::Response> m_components{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Adds new subcomponent
         *
         * @param subcomponent New subcomponent
         * */
        void add_subcomponent(const ComponentDTO::Response& subcomponent);

        /*!
         * Gets list of subcomponents
         *
         * @return Vector of subcomponents
         * */
        const std::vector<ComponentDTO::Response>& get_subcomponents()
            const { return m_components; }

        /*!
         * Desarializes component response object to JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * Sets component type
         *
         * @param type component type
         * */
        void set_type(const std::string& type) { m_type = type; }

        /*!
         * Gets component type
         *
         * @return Component type
         * */
        const std::string& get_type() const { return m_type; }

        /*!
         * Sets component name
         *
         * @param name Component name
         * */
        void set_name(const std::string& name) { m_name = name; }

        /*!
         * Gets component name
         *
         * @return Component name
         * */
        const std::string& get_name() const { return m_name; }

        virtual ~Response();
    };

    virtual ~ComponentDTO();
};

}
}
}

#endif /* COMPONENT_DTO */

