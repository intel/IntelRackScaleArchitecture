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
 * @file core/dto/compute/network_interface_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef COMPUTE_NETWORK_INTERFACE_INFO_DTO_HPP
#define COMPUTE_NETWORK_INTERFACE_INFO_DTO_HPP

#include "core/dto/ipv4_address_dto.hpp"
#include "core/dto/ipv6_address_dto.hpp"
#include "core/dto/neighbor_info_dto.hpp"
#include "core/dto/status_dto.hpp"
#include "core/dto/fru_info_dto.hpp"
#include "core/dto/oem_data_dto.hpp"
#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Network namespace */
namespace compute {

/*! Network interface information request object */
class NetworkInterfaceInfoDTO {
public:
    /*! Copy constructor */
    NetworkInterfaceInfoDTO(const NetworkInterfaceInfoDTO &) = default;

    /*! Assigment constructor */
    NetworkInterfaceInfoDTO& operator=(const NetworkInterfaceInfoDTO &) = default;

    /*! Default constructor */
    NetworkInterfaceInfoDTO() {}

    /*! Network interface information DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::uint32_t m_interface{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets component UUID
         *
         * @param component Component UUID
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets component UUID
         *
         * @return Component UUID
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * Sets network interface id
         *
         * @param interface Network interface id
         * */
        void set_interface(std::uint32_t interface) {
            m_interface = interface;
        }

        /*!
         * Gets network interface id
         *
         * @return Network interface id
         * */
        std::uint32_t get_interface() const {
            return m_interface;
        }

        virtual ~Request();
    };

    /*! Network interface information DTO response */
    class Response : public ResponseDTO {
        uint32_t m_frame_size{};
        std::string m_link_technology{"Ethernet"};
        uint32_t m_speed_gbps{};
        bool m_autosense{};
        std::string m_mac_address{};
        IPv4AddressDTO::Response m_ipv4_address{};
        IPv6AddressDTO::Response m_ipv6_address{};
        NeighborInfoDTO::Response m_neighbor_info{};
        bool m_vlan_enable{};
        uint32_t m_vlan_count{};
        StatusDTO::Response m_status{};
        FRUInfoDTO::Response m_fru_info{};
        OEMDataDTO::Response m_oem_data{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes network interface information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Sets frame size
         *
         * @param frame_size frame size
         */
        void set_frame_size(uint32_t frame_size) {
            m_frame_size = frame_size;
        }

        /*!
         * @brief Gets frame size
         *
         * @return frame size
         */
        uint32_t get_frame_size() const {
            return m_frame_size;
        }

        /*!
         * @brief Sets link technology
         *
         * @param link_technology link technology
         */
        void set_link_technology(const std::string& link_technology) {
            m_link_technology = link_technology;
        }

        /*!
         * @brief Gets link technology
         *
         * @return link technology
         */
        const std::string& get_link_technology() const {
            return m_link_technology;
        }

        /*!
         * @brief Sets speed in Gbps
         *
         * @param speed_gbps Speed in Gbps
         */
        void set_speed_gbps(uint32_t speed_gbps) {
            m_speed_gbps = speed_gbps;
        }

        /*!
         * @brief Gets speed in Gbps
         *
         * @return Speed in Gbps
         */
        uint32_t get_speed() const {
            return m_speed_gbps;
        }

        /*!
         * @brief Sets autosense flag
         *
         * @param autosense autosense flag
         */
        void set_autosense(bool autosense) {
            m_autosense = autosense;
        }

        /*!
         * @brief Gets autosense flag
         *
         * @return autosense flag
         */
        bool get_autosense() const {
            return m_autosense;
        }

        /*!
         * @brief Sets MAC address
         *
         * @param mac_address MAC address
         */
        void set_mac_address(const std::string& mac_address) {
            m_mac_address = mac_address;
        }

        /*!
         * @brief Gets MAC address
         *
         * @return MAC address
         */
        const std::string& get_mac_address() const {
            return m_mac_address;
        }

        /*!
         * @brief Sets IPv4 address
         *
         * @param ipv4_address IPv4 address
         */
        void set_ipv4_address(const IPv4AddressDTO::Response& ipv4_address) {
            m_ipv4_address = ipv4_address;
        }

        /*!
         * @brief Gets IPv4 address
         *
         * @return IPv4 address
         */
        const IPv4AddressDTO::Response& get_ipv4_address() const {
            return m_ipv4_address;
        }

        /*!
         * @brief Sets IPv6 address
         *
         * @param ipv6_address IPv6 address
         */
        void set_ipv6_address(const IPv6AddressDTO::Response& ipv6_address) {
            m_ipv6_address = ipv6_address;
        }

        /*!
         * @brief Gets IPv6 address
         *
         * @return IPv6 address
         */
        const IPv6AddressDTO::Response& get_ipv6_address() const {
            return m_ipv6_address;
        }

        /*!
         * @brief Sets neighbor info
         *
         * @param neighbor_info Neighbor info
         */
        void set_neighbor_info(
            const NeighborInfoDTO::Response& neighbor_info) {
            m_neighbor_info = neighbor_info;
        }

        /*!
         * @brief Gets neighbor info
         *
         * @return neighbor info
         */
        const NeighborInfoDTO::Response& get_neighbor_info() const {
            return m_neighbor_info;
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
         * @brief Sets vlan count
         *
         * @param vlan_count vlan count
         */
        void set_vlan_count(uint32_t vlan_count) {
            m_vlan_count = vlan_count;
        }

        /*!
         * @brief Gets vlan count
         *
         * @return vlan count
         */
        uint32_t get_vlan_count() const {
             return m_vlan_count;
        }

        /*!
         * Sets status
         *
         * @param status Status
         * */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * Gets status
         *
         * @return Status
         * */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * Sets FRU info
         *
         * @param fru_info FRU info
         * */
        void set_fru_info(const FRUInfoDTO::Response& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * Gets FRU info
         *
         * @return FRU info
         * */
        const FRUInfoDTO::Response& get_fru_info() const {
            return m_fru_info;
        }

        /*!
         * Sets OEM specific data
         *
         * @param oem_data OEM specific data
         * */
        void set_oem_data(const OEMDataDTO::Response& oem_data) {
            m_oem_data = oem_data;
        }

        /*!
         * Gets OEM specific data
         *
         * @return OEM specific data
         * */
        const OEMDataDTO::Response& get_oem_data() const {
            return m_oem_data;
        }

        virtual ~Response();
    };

    ~NetworkInterfaceInfoDTO();
};

}
}
}
}

#endif /* COMPUTE_NETWORK_INTERFACE_INFO_DTO_HPP */
