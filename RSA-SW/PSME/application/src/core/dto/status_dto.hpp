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
 * @file core/dto/status_dto.hpo
 * @brief Status response transfer obejct declaration.
 *
 * */

#ifndef STATUS_DTO_HPP
#define STATUS_DTO_HPP

#include "response_dto.hpp"

#include <json/json.h>

#include <string>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Dto namespace */
namespace dto {

/*! Component transfer object */
class StatusDTO {
public:
    /*! Copy constructor */
    StatusDTO(const StatusDTO &) = default;

    /*! Assigment constructor */
    StatusDTO& operator=(const StatusDTO &) = default;

    /*! Default constructor */
    StatusDTO() {}

    class Response : ResponseDTO {
        std::string m_state{};
        std::string m_health{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes status response object from JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * Sets state
         *
         * @param state State
         * */
        void set_state(const std::string& state) { m_state = state; }

        /*!
         * Gets state
         *
         * @return State
         * */
        const std::string& get_state() const { return m_state; }

        /*!
         * Sets health
         *
         * @param health Health
         * */
        void set_health(const std::string& health) { m_health = health; }

        /*!
         * Gets health
         *
         * @return Health
         * */
        const std::string& get_health() const { return m_health; }

        virtual ~Response();
    };

    virtual ~StatusDTO();
};

}
}
}

#endif /* STATUS_DTO_HPP */

