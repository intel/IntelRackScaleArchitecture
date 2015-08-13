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
 * @file core/dto/network/switch_port_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_NETWORK_SWITCH_PORT_INFO_DTO_HPP
#define PSME_CORE_DTO_NETWORK_SWITCH_PORT_INFO_DTO_HPP

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

/*! Switch Port information request object */
class SwitchPortInfoDTO {
public:
    /*! Copy constructor */
    SwitchPortInfoDTO(const SwitchPortInfoDTO &) = default;

    /*! Assigment constructor */
    SwitchPortInfoDTO& operator=(const SwitchPortInfoDTO &) = default;

    /*! Default constructor */
    SwitchPortInfoDTO() {}

    /*! Switch information DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::string m_port_identifier{};
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

        virtual ~Request();
    };

    /*! Switch information DTO response */
    class Response : public ResponseDTO {
        StatusDTO::Response m_status{};
        std::string m_port_identifier{};
        std::string m_link_technology{"Unknown"};
        uint32_t m_link_speed_gbps{};
        uint32_t m_max_speed_gbps{};
        std::string m_administrative_state{"None"};
        std::string m_operational_state{"Unknown"};
        uint32_t m_port_width{};
        uint32_t m_frame_size{};
        bool m_autosense{};
        bool m_is_management_port{};
        uint32_t m_last_error_code{};
        bool m_error_cleared{};
        std::string m_last_state_change_time{};
        std::string m_mac_address{};
        IPv4AddressDTO::Response m_ipv4_address{};
        IPv6AddressDTO::Response m_ipv6_address{};
        NeighborInfoDTO::Response m_neighbor_info{};
        bool m_vlan_enable{};
        OEMDataDTO::Response m_oem{};

    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes switch port information response object to JSON
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
         * @brief Sets port identifier
         *
         * @param port_identifier port identifier
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
         * @brief Set administrative state
         *
         * @param administrative_state Administrative state
         */
        void set_administrative_state(const std::string& administrative_state) {
            m_administrative_state = administrative_state;
        }

        /*!
         * @brief Get administrative state
         *
         * @return Administrative state
         */
        const std::string& get_administrative_state() const {
             return m_administrative_state;
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
         * @brief Sets link speed Gbps
         *
         * @param link_speed_gbps link speed Gbps
         */
        void set_link_speed_gbps(uint32_t link_speed_gbps) {
            m_link_speed_gbps = link_speed_gbps;
        }

        /*!
         * @brief Gets link speed Gbps
         *
         * @return link speed Gbps
         */
        uint32_t get_link_speed_gbps() const {
            return m_link_speed_gbps;
        }

        /*!
         * @brief Sets max speed Gbps
         *
         * @param max_speed_gbps max speed Gbps
         */
        void set_max_speed_gbps(uint32_t max_speed_gbps) {
            m_max_speed_gbps = max_speed_gbps;
        }

        /*!
         * @brief Gets max speed Gbps
         *
         * @return max speed Gbps
         */
        uint32_t get_max_speed_gbps() const {
            return m_max_speed_gbps;
        }

        /*!
         * @brief Sets operational state
         *
         * @param operational_state operational state
         */
        void set_operational_state(const std::string& operational_state) {
            m_operational_state = operational_state;
        }

        /*!
         * @brief Gets operational state
         *
         * @return operational state
         */
        const std::string& get_operational_state() const {
            return m_operational_state;
        }

        /*!
         * @brief Sets port width
         *
         * @param port_width port width
         */
        void set_port_width(uint32_t port_width) {
            m_port_width = port_width;
        }

        /*!
         * @brief Gets port width
         *
         * @return port width
         */
        uint32_t get_port_width() const {
            return m_port_width;
        }

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
         * @brief Sets port as management port
         *
         * @param is_management_port is management port flag
         */
        void set_is_management_port(bool is_management_port) {
            m_is_management_port = is_management_port;
        }

        /*!
         * @brief Gets is management port flag
         *
         * @return is management port flag
         */
        bool get_is_management_port() const {
            return m_is_management_port;
        }

        /*!
         * @brief Sets code of last error detected
         *
         * @param last_error_code code of last error detected
         */
        void set_last_error_code(uint32_t last_error_code) {
            m_last_error_code = last_error_code;
        }

        /*!
         * @brief Gets code of last error detected
         *
         * @return code of last error detected
         */
        uint32_t get_last_error_code() const {
            return m_last_error_code;
        }

        /*!
         * @brief Sets error cleared flag
         *
         * @param error_cleared error cleared flag
         */
        void set_error_cleared(bool error_cleared) {
            m_error_cleared = error_cleared;
        }

        /*!
         * @brief Gets error cleared flag
         *
         * @return error cleared flag
         */
        bool get_error_cleared() const {
            return m_error_cleared;
        }
        /*!
         * @brief Sets time of last state change
         *
         * @param last_state_change_time time of last state change
         */
        void set_last_state_change_time(const std::string& last_state_change_time) {
            m_last_state_change_time = last_state_change_time;
        }

        /*!
         * @brief Gets time of last state change
         *
         * @return time of last state change
         */
        const std::string& get_last_state_change_time() const {
            return m_last_state_change_time;
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
         * @brief Sets neighbor info
         *
         * @param neighbor_info neighbor info
         */
        void set_neighbor_info(const NeighborInfoDTO::Response& neighbor_info) {
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
         * @brief Sets VLAN enabled flag
         *
         * @param vlan_enable VLAN enabled flag
         */
        void set_vlan_enable(bool vlan_enable) {
            m_vlan_enable = vlan_enable;
        }

        /*!
         * @brief Gets VLAN enabled flag
         *
         * @return VLAN enabled flag
         */
        bool get_vlan_enable() const {
            return m_vlan_enable;
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

    ~SwitchPortInfoDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_SWITCH_PORT_INFO_DTO_HPP */
