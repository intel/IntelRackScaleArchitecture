/*!
 * Copyright (c) 2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file command/network/get_remote_switch_info.hpp
 * @brief Generic network GetRemoteSwitchInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_NETWORK_GET_REMOTE_SWITCH_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_NETWORK_GET_REMOTE_SWITCH_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"

#include <string>
#include <vector>

namespace agent_framework {
namespace command {
namespace network {

/* Forward declaration */
namespace json { class GetRemoteSwitchInfo; }

using agent_framework::generic::Status;

using std::vector;

/*! Generic network command GetRemoteSwitchInfo */
class GetRemoteSwitchInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Network";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getRemoteSwitchInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetRemoteSwitchInfo();
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
        friend class json::GetRemoteSwitchInfo;
        std::string m_component{};
        std::string m_switch_id{};
    public:
        /*!
         * @brief Get component from request
         * @return uuid string
         * */
        const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get switch id from request
         * @return Switch id
         * */
        const string& get_switch_id() const {
            return m_switch_id;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:

        /*! Hop object */
        class Hop {
            string m_port_identifier{};
            string m_ipv4_address{};
            string m_ipv6_address{};
            int32_t m_metric{};
        public:
            /*!
             * @brief Constructor
             * @param[in] port_identifier Hop port
             * @param[in] ipv4_address Hop IPv4 address
             * @param[in] ipv6_address Hop IPv6 address
             * @param[in] metric Hop metric
             * */
            Hop(const string& port_identifier = "",
                const string& ipv4_address = "",
                const string& ipv6_address = "", int32_t metric = 0)
                : m_port_identifier(port_identifier),
                m_ipv4_address(ipv4_address), m_ipv6_address(ipv6_address),
                m_metric(metric) {}

            /*! Default copy constructor */
            Hop(const Hop&) = default;

            /*! Default assignment operator */
            Hop& operator=(const Hop&) = default;

            /*!
             * @brief Set hop port
             * @param[in] port_identifier Hop port
             * */
            void set_port_identifier(const string& port_identifier) {
                m_port_identifier = port_identifier;
            }

            /*!
             * @brief Get hop port
             * @return Hop port
             * */
            const string& get_port_identifier() const {
                return m_port_identifier;
            }

            /*!
             * @brief Set hop IPv4 address
             * @param[in] address Hop IPv4 address
             * */
            void set_ipv4_address(const string& address) {
                m_ipv4_address = address;
            }

            /*!
             * @brief Get hop IPv4 address
             * @return Hop address
             * */
            const string& get_ipv4_address() const {
                return m_ipv4_address;
            }

            /*!
             * @brief Set hop IPv6 address
             * @param[in] address Hop IPv6 address
             * */
            void set_ipv6_address(const string& address) {
                m_ipv6_address = address;
            }

            /*!
             * @brief Get hop IPv6 address
             * @return Hop address
             * */
            const string& get_ipv6_address() const {
                return m_ipv6_address;
            }

            /*!
             * @brief Set hop metric
             * @param[in] metric Hop metric
             * */
            void set_metric(int32_t metric) {
                m_metric = metric;
            }

            /*!
             * @brief Get hop metric
             * @return Hop metric
             * */
            int32_t get_metric() const {
                return m_metric;
            }
        };

        /*!
         * @brief Set status from request
         * @param status switch status
         * */
        void set_status(const Status& status) {
            m_status = status;
        }

        /*!
         * @brief Set mac address from request
         * @param mac_address switch mac address
         * */
        void set_mac_address(const std::string& mac_address) {
            m_mac_address = mac_address;
        }

        /*!
         * @brief Add next hop
         * @param next_hop Next hop
         * */
        void add_next_hop(const Hop& next_hop) {
            m_next_hops.push_back(next_hop);
        }

        ~Response();

    protected:
        friend class json::GetRemoteSwitchInfo;
        Status m_status{};
        string m_mac_address{};
        vector<Hop> m_next_hops{};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_NETWORK_GET_REMOTE_SWITCH_INFO_HPP */
