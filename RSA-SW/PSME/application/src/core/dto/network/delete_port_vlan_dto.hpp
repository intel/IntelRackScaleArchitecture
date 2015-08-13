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
 * @file core/dto/network/delete_port_vlan_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_NETWORK_DELETE_PORT_VLAN_DTO_HPP
#define PSME_CORE_DTO_NETWORK_DELETE_PORT_VLAN_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/oem_data_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Network namespace */
namespace network {

/*! VLAN Port request object */
class DeletePortVlanDTO {
public:
    /*! Copy constructor */
    DeletePortVlanDTO(const DeletePortVlanDTO &) = default;

    /*! Assigment constructor */
    DeletePortVlanDTO& operator=(const DeletePortVlanDTO &) = default;

    /*! Default constructor */
    DeletePortVlanDTO() {}

    /*! DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::string m_port_identifier{};
        std::string m_vlan_identifier{};
        OEMDataDTO::Request m_oem{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets component id
         *
         * @param[in] component Component id
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets component id
         *
         * @return Component id
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * Sets port identifier
         *
         * @param[in] port_identifier Port identifier
         * */
        void set_port_identifier(const std::string& port_identifier) {
            m_port_identifier = port_identifier;
        }

        /*!
         * Gets port identifier
         *
         * @return port identifier
         * */
        const std::string& get_port_identifier() const {
            return m_port_identifier;
        }

        /*!
         * Sets VLAN identifier
         *
         * @param[in] vlan_identifier VLAN identifier
         * */
        void set_vlan_identifier(const std::string& vlan_identifier) {
            m_vlan_identifier = vlan_identifier;
        }

        /*!
         * Gets VLAN identifier
         *
         * @return VLAN identifier
         * */
        const std::string& get_vlan_identifier() const {
            return m_vlan_identifier;
        }

        /*!
         * @brief Sets OEM data
         *
         * @param oem OEM data
         */
        void set_oem(const OEMDataDTO::Request& oem) {
            m_oem = oem;
        }

        /*!
         * @brief Gets OEM data
         *
         * @return OEM data
         */
        const OEMDataDTO::Request& get_oem() const {
            return m_oem;
        }

        virtual ~Request();
    };

    /*! DTO response */
    class Response : public ResponseDTO {
        OEMDataDTO::Response m_oem{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes VLAN port response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Sets oem
         *
         * @param oem oem
         */
        void set_oem(const OEMDataDTO::Response& oem) {
            m_oem = oem;
        }

        /*!
         * @brief Gets oem
         *
         * @return oem
         */
        const OEMDataDTO::Response& get_oem() const {
            return m_oem;
        }

        virtual ~Response();
    };

    ~DeletePortVlanDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_DELETE_PORT_VLAN_DTO_HPP */
