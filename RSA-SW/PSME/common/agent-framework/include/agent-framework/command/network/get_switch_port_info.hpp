/*!
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
 *
 * @file command/network/get_switch_port_info.hpp
 * @brief Generic network GetSwitchPortInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_NETWORK_GET_SWITCH_PORT_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_NETWORK_GET_SWITCH_PORT_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/ipv4_address.hpp"
#include "agent-framework/module/ipv6_address.hpp"
#include "agent-framework/module/neighbor_info.hpp"
#include <string>

namespace agent_framework {
namespace command {
namespace network {

/* Forward declaration */
namespace json { class GetSwitchPortInfo; }

using Status = agent_framework::generic::Status;
using OEMData = agent_framework::generic::OEMData;
using IPv4Address = agent_framework::generic::IPv4Address;
using IPv6Address = agent_framework::generic::IPv6Address;
using NeighborInfo = agent_framework::generic::NeighborInfo;

/*! Generic network command GetSwitchInfoInfo */
class GetSwitchPortInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Network";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getSwitchPortInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetSwitchPortInfo();
protected:
    /*!
     * @brief Execute command with givent command arguments
     *
     * @param[in]   in      Input command argument
     * @param[out]  out     Output command argument
     * */
    void execute(const Argument& in, Argument& out) override final {
        execute(static_cast<const Request&>(in), static_cast<Response&>(out));
    }
public:
    /*! Argument request to execute */
    class Request : public Argument {
    private:
        friend class json::GetSwitchPortInfo;
        string m_component{};
        std::string m_port_identifier{};
    public:
        /*!
         * @brief Get component from request
         * @return uuid string
         * */
        const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get port identifier
         * @return port identifier
         * */
        const std::string& get_port_identifier() const {
            return m_port_identifier;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    private:
        friend class json::GetSwitchPortInfo;
        Status m_status{};
        std::string m_port_identifier{};
        std::string m_port_type{};
        std::string m_link_technology{};
        std::uint32_t m_link_speed{};
        std::uint32_t m_max_speed{};
        std::string m_link_state{};
        std::string m_operational_state{"Unknown"};
        std::string m_administrative_state{"None"};
        std::uint32_t m_port_width{};
        std::uint32_t m_frame_size{};
        bool m_autosense{};
        bool m_is_management_port{};
        std::uint32_t m_last_error_code{};
        bool m_error_cleared{};
        std::string m_last_state_change_time{};
        std::string m_mac_address{};
        IPv4Address m_ipv4address{};
        IPv6Address m_ipv6address{};
        NeighborInfo m_neighbor_info{};
        bool m_vlan_enable{};
        OEMData m_oem_data{};

    public:
        /*!
         * @brief Switch port status
         * @param[in] status Port status
         * */
        void set_status(const Status& status) {
            m_status = status;
        }

        /*!
         * @brief Set port identifier
         * @param port_identifier port identifier
         */
        void set_port_identifier(const std::string& port_identifier) {
            m_port_identifier = port_identifier;
        }

        /*!
         * @brief Set port type
         * @param port_type port type
         */
        void set_port_type(const std::string& port_type) {
            m_port_type = port_type;
        }

        /*!
         * @brief Set link technology
         * @param link_technology link technology
         */
        void set_link_technology(const std::string& link_technology) {
            m_link_technology = link_technology;
        }

        /*!
         * @brief Set link speed
         * @param link_speed link speed
         */
        void set_link_speed(const std::uint32_t link_speed) {
            m_link_speed = link_speed;
        }

        /*!
         * @brief Set max speed
         * @param max_speed max speed
         */
        void set_max_speed(const std::uint32_t max_speed) {
            m_max_speed = max_speed;
        }

        /*!
         * @brief Set operational state
         * @param operational_state operational state
         */
        void set_operational_state(const std::string& operational_state) {
            m_operational_state = operational_state;
        }

        /*!
         * @brief Set administrative state
         * @param administrative_state Administrative state
         */
        void set_administrative_state(const std::string& administrative_state) {
            m_administrative_state = administrative_state;
        }

        /*!
         * @brief Set port width
         * @param port_width port width
         */
        void set_port_width(const std::uint32_t port_width) {
            m_port_width = port_width;
        }

        /*!
         * @brief Set frame size
         * @param frame_size frame size
         */
        void set_frame_size(const std::uint32_t frame_size) {
            m_frame_size = frame_size;
        }

        /*!
         * @brief Set autosense
         * @param autosense autosense
         */
        void set_autosense(const bool autosense) {
            m_autosense = autosense;
        }

        /*!
         * @brief Set is management port
         * @param is_management_port is management port
         */
        void set_is_management_port(const bool is_management_port) {
            m_is_management_port = is_management_port;
        }

        /*!
         * @brief Set last error code
         * @param last_error_code last error code
         */
        void set_last_error_code(const std::uint32_t last_error_code) {
            m_last_error_code = last_error_code;
        }

        /*!
         * @brief Set error cleared
         * @param error_cleared error cleared
         */
        void set_error_cleared(const bool error_cleared) {
            m_error_cleared = error_cleared;
        }

        /*!
         * @brief Set last state change time
         * @param last_state_change_time last state change time
         */
        void
        set_last_state_change_time(const std::string& last_state_change_time) {
            m_last_state_change_time = last_state_change_time;
        }

        /*!
         * @brief Set mac address
         * @param mac_address mac address
         */
        void set_mac_address(const std::string& mac_address) {
            m_mac_address = mac_address;
        }

        /*!
         * @brief Set IPv4 address
         * @param ipv4address IPv4 address
         */
        void set_ipv4address(const IPv4Address& ipv4address) {
            m_ipv4address  = ipv4address;
        }

        /*!
         * @brief Set IPv6 address
         * @param ipv6address IPv6 address
         */
        void set_ipv6address(const IPv6Address& ipv6address) {
            m_ipv6address = ipv6address;
        }

        /*!
         * @brief Set neighbor information
         * @param neighbor_info neighbor info
         */
        void set_neighbor_info(const NeighborInfo& neighbor_info) {
            m_neighbor_info = neighbor_info;
        }

        /*!
         * @brief Set vlan enable
         * @param vlan_enable vlan enable
         */
        void set_vlan_enable(const bool vlan_enable) {
            m_vlan_enable = vlan_enable;
        }

        /*!
         * @brief Set oem data
         * @param oem_data oem data
         */
        void set_oem(const OEMData& oem_data) {
            m_oem_data = oem_data;
        }

        ~Response();
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_NETWORK_GET_SWITCH_PORT_INFO_HPP */
