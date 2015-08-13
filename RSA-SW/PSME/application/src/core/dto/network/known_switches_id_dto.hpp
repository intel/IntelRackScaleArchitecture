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
 * @file core/dto/network/known_switches_id_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_NETWORK_KNOWN_SWITCHES_ID_DTO_HPP
#define PSME_CORE_DTO_NETWORK_KNOWN_SWITCHES_ID_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Network namespace */
namespace network {

/*! Known Switches ID information DTO object */
class KnownSwitchesIdDTO {
public:
    /*! Copy constructor */
    KnownSwitchesIdDTO(const KnownSwitchesIdDTO &) = default;

    /*! Assigment constructor */
    KnownSwitchesIdDTO& operator=(const KnownSwitchesIdDTO &) = default;

    /*! Default constructor */
    KnownSwitchesIdDTO() {}

    /*! Known Switches ID information DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets switch UUID
         *
         * @param component Switch UUID
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets switch UUID
         *
         * @return Switch UUID
         * */
        const std::string& get_component() const {
            return m_component;
        }

        virtual ~Request();
    };

    /*! Known Switches ID information DTO object */
    class Response : public ResponseDTO {
        std::vector<std::string> m_switch_identifiers{};

    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Deserialize known switches information response from JSON to object.
         *
         * @param data JSON to be deserialized to object.
         * */
        void to_object(const Json::Value& data);

        /*!
         * Gets list of switch identifiers.
         * @return vector of string which contain switch identifiers.
         */
        const std::vector<std::string>& get_switch_identifiers() const {
            return m_switch_identifiers;
        }

        /*!
         * Sets list of switch identifiers.
         * @param ids vector of strings with switch identifiers.
         */
        void get_switch_identifiers(const std::vector<std::string>& ids) {
            m_switch_identifiers = ids;
        }

        virtual ~Response();
    };

    ~KnownSwitchesIdDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_KNOWN_SWITCHES_ID_DTO_HPP */
