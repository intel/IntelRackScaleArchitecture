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
 * @file core/dto/neighbor_info_dto.hpo
 * @brief Neighbor info transfer object declaration.
 *
 * */
#ifndef NEIGHBOR_INFO_DTO_HPP
#define NEIGHBOR_INFO_DTO_HPP

#include "core/dto/response_dto.hpp"

#include <json/json.h>
#include <string>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {

/*! Neighbor info transfer object */
class NeighborInfoDTO {
public:

    /*! Copy constructor */
    NeighborInfoDTO(const NeighborInfoDTO &) = default;

    /*! Assigment constructor */
    NeighborInfoDTO& operator=(const NeighborInfoDTO &) = default;

    /*! Default constructor */
    NeighborInfoDTO() {}

    class Response : ResponseDTO {
        std::string m_switch_identifier{};
        std::string m_port_identifier{};

    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * @brief Sets switch identifier
         *
         * @param switch_identifier switch identifier
         */
        void set_switch_identifier(const std::string& switch_identifier) {
            m_switch_identifier = switch_identifier;
        }

        /*!
         * @brief Gets switch identifier
         *
         * @return switch identifier
         */
        const std::string& get_switch_identifier() const {
            return m_switch_identifier;
        }

        /*!
         * @brief Sets port identifier
         *
         * @param port_identifier port identifier
         *
         */
        void set_port_identifier(const std::string& port_identifier) {
            m_port_identifier = port_identifier;
        }

        /*!
         * @brief Gets port identifier
         *
         * @return port identifier
         */
        const std::string& get_port_identifier() const {
            return m_port_identifier;
        }

        /*!
         * Desarializes NeighborInfo object from JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        virtual ~Response();
    };

    virtual ~NeighborInfoDTO();

};

}
}
}

#endif // NEIGHBOR_INFO_DTO_HPP
