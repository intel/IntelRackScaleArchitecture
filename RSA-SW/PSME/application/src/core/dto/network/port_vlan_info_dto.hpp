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
 * @file core/dto/network/port_vlan_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_NETWORK_PORT_VLAN_INFO_DTO_HPP
#define PSME_CORE_DTO_NETWORK_PORT_VLAN_INFO_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/status_dto.hpp"
#include "core/dto/ipv4_address_dto.hpp"
#include "core/dto/ipv6_address_dto.hpp"
#include "core/dto/neighbor_info_dto.hpp"
#include "core/dto/oem_data_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Network namespace */
namespace network {

/*! VLAN Port information request object */
class PortVlanInfoDTO {
public:
    /*! Copy constructor */
    PortVlanInfoDTO(const PortVlanInfoDTO &) = default;

    /*! Assigment constructor */
    PortVlanInfoDTO& operator=(const PortVlanInfoDTO &) = default;

    /*! Default constructor */
    PortVlanInfoDTO() {}

    /*! Switch information DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::string m_port_identifier{};
        std::string m_vlan_identifier{};
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

        /*!
         * Sets switch port identifier
         *
         * @param port_identifier Switch port identifier
         * */
        void set_port_identifier(const std::string& port_identifier) {
            m_port_identifier = port_identifier;
        }

        /*!
         * Gets switch port identifier
         *
         * @return Switch port identifier
         * */
        const std::string& get_port_identifier() const {
            return m_port_identifier;
        }

        /*!
         * Sets VLAN identifier
         *
         * @param vlan_identifier VLAN identifier
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

        virtual ~Request();
    };

    /*! Port VLAN information DTO response */
    class Response : public ResponseDTO {
        StatusDTO::Response m_status{};
        uint32_t m_vlan_id{};
        bool m_vlan_enable{};
        bool m_tagged{};
        IPv4AddressDTO::Response m_ipv4_address{};
        IPv6AddressDTO::Response m_ipv6_address{};
        OEMDataDTO::Response m_oem{};

    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes VLAN port information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Sets component status
         *
         * @param status component status
         */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * @brief Gets component status
         *
         * @return component status
         */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * @brief Sets vlan id
         *
         * @param vlan_id vlan id
         */
        void set_vlan_id(uint32_t vlan_id) {
            m_vlan_id = vlan_id;
        }

        /*!
         * @brief Gets vlan id
         *
         * @return vlan id
         */
        uint32_t get_vlan_id() const {
            return m_vlan_id;
        }

        /*!
         * @brief Sets VLAN enable flag
         *
         * @param vlan_enable VLAN enable flag
         */
        void set_vlan_enable(bool vlan_enable) {
            m_vlan_enable = vlan_enable;
        }

        /*!
         * @brief Gets VLAN enable flag
         *
         * @return VLAN enable flag
         */
        bool get_vlan_enable() const {
            return m_vlan_enable;
        }

        /*!
         * @brief Sets tagged VLAN flag
         *
         * @param tagged tagged VLAN flag
         */
        void set_tagged(bool tagged) {
            m_tagged = tagged;
        }

        /*!
         * @brief Gets tagged VLAN flag
         *
         * @return tagged VLAN flag
         */
        bool get_tagged() const {
            return m_tagged;
        }

        /*!
         * @brief Sets ipv4 address
         *
         * @param ipv4_address ipv4 address
         */
        void set_ipv4_address(const IPv4AddressDTO::Response& ipv4_address) {
            m_ipv4_address = ipv4_address;
        }

        /*!
         * @brief Gets ipv4 address
         *
         * @return ipv4 address
         */
        const IPv4AddressDTO::Response& get_ipv4_address() const {
            return m_ipv4_address;
        }

        /*!
         * @brief Sets ipv6 address
         *
         * @param ipv6_address ipv6 address
         */
        void set_ipv6_address(const IPv6AddressDTO::Response& ipv6_address) {
            m_ipv6_address = ipv6_address;
        }

        /*!
         * @brief Gets ipv6 address
         *
         * @return ipv6 address
         */
        const IPv6AddressDTO::Response& get_ipv6_address() const {
            return m_ipv6_address;
        }

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

    ~PortVlanInfoDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_PORT_VLAN_INFO_DTO_HPP */
