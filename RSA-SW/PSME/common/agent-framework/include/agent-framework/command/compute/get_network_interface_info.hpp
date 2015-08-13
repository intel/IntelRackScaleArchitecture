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
 * @file command/compute/get_network_interface_info.hpp
 * @brief Generic compute GetProcessorInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_NETWORK_INTERFACE_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_NETWORK_INTERFACE_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/ipv4_address.hpp"
#include "agent-framework/module/ipv6_address.hpp"
#include "agent-framework/module/neighbor_info.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace compute {

/* Forward declaration */
namespace json { class GetNetworkInterfaceInfo; }

using std::string;

using OEMData = agent_framework::generic::OEMData;
using Status = agent_framework::generic::Status;
using IPv4Address = agent_framework::generic::IPv4Address;
using IPv6Address = agent_framework::generic::IPv6Address;
using NeighborInfo = agent_framework::generic::NeighborInfo;
using OEMDataConstUniquePtr = OEMData::OEMDataConstUniquePtr;

/*! Generic compute command GetNetworkInterfaceInfo */
class GetNetworkInterfaceInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr char AGENT[] = "Compute";

    /*! Tag string for identify command */
    static constexpr char TAG[] = "getNetworkInterfaceInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetNetworkInterfaceInfo();
protected:
    /*!
     * @brief Execute command with given command arguments
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
        friend class json::GetNetworkInterfaceInfo;
        string m_component{};
        unsigned int m_interface{};
    public:
        /*!
         * @brief Get component id from request
         * @return Computer system id string (UUID)
         * */
        inline const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get interface number
         * @return Interface number
         * */
        inline unsigned int get_interface() const {
            return m_interface;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:
        /*!
         * @brief Set frame size
         * @param[in] frame_size Frame size
         * */
        void set_frame_size(uint32_t frame_size) {
            m_frame_size = frame_size;
        }

        /*!
         * @brief Set link technology
         * @param[in] link_technology Link technology
         * */
        void set_link_technology(const string& link_technology) {
            m_link_technology = link_technology;
        }

        /*!
         * @brief Set speed in Gbps
         * @param[in] speed_gbps Speed in Gbps
         * */
        void set_speed_gbps(uint32_t speed_gbps) {
            m_speed_gbps = speed_gbps;
        }

        /*!
         * @brief Set autosense
         * @param[in] autosense Autosense
         * */
        void set_autosense(bool autosense) {
            m_autosense = autosense;
        }

        /*!
         * @brief Set MAC address
         * @param[in] mac_address MAC address
         * */
        void set_mac_address(const string& mac_address) {
            m_mac_address = mac_address;
        }

        /*!
         * @brief Set VLAN enable
         * @param[in] vlan_enable VLAN enable
         * */
        void set_vlan_enable(bool vlan_enable) {
            m_vlan_enable = vlan_enable;
        }

        /*!
         * @brief Set VLAN count
         * @param[in] vlan_count VLAN count
         * */
        void set_vlan_count(uint32_t vlan_count) {
            m_vlan_count = vlan_count;
        }

        /*!
         * @brief Set IPv4 address for response
         * @param[in] ipv4_address IPv4 address
         * */
        void set_ipv4_address(const IPv4Address& ipv4_address) {
            m_ipv4_address = ipv4_address;
        }

        /*!
         * @brief Set IPv6 address for response
         * @param[in] ipv6_address IPv6 address
         * */
        void set_ipv6_address(const IPv6Address& ipv6_address) {
            m_ipv6_address = ipv6_address;
        }

        /*!
         * @brief Set neighbor info for response
         * @param[in] neighbor_info Neighbor info
         * */
        void set_neighbor_info(const NeighborInfo& neighbor_info) {
            m_neighbor_info = neighbor_info;
        }

        /*!
         * @brief Set Status for response
         * @param[in] status Status
         * */
        void set_status(const Status& status) {
            m_status = status;
        }

        /*!
         * @brief Set OEM specific data for response
         * @param[in] oem OEM specific data
         * */
        void set_oem(const OEMData* oem) {
            m_oem.reset(oem);
        }


        ~Response();
    private:
        friend class json::GetNetworkInterfaceInfo;
        uint32_t m_frame_size{};
        string m_link_technology{};
        uint32_t m_speed_gbps{};
        bool m_autosense{};
        string m_mac_address{};
        bool m_vlan_enable{};
        uint32_t m_vlan_count{};
        Status m_status{};
        IPv4Address m_ipv4_address{};
        IPv6Address m_ipv6_address{};
        NeighborInfo m_neighbor_info{};
        OEMDataConstUniquePtr m_oem{new OEMData};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_NETWORK_INTERFACE_INFO_HPP */
