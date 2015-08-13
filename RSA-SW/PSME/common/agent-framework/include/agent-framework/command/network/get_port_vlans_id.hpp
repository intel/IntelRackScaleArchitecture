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
 * @file command/network/get_port_vlans_id.hpp
 * @brief Generic network GetPortVlansId command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_NETWORK_GET_PORT_VLANS_ID_HPP
#define AGENT_FRAMEWORK_COMMAND_NETWORK_GET_PORT_VLANS_ID_HPP

#include "agent-framework/command/command.hpp"

#include <vector>
#include <string>

namespace agent_framework {
namespace command {
namespace network {

/* Forward declaration */
namespace json { class GetPortVlansId; }

/*! Generic network command GetPortVlansId */
class GetPortVlansId : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Network";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getPortVlansId";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetPortVlansId();
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
        friend class json::GetPortVlansId;
        std::string m_component{};
        string m_port_identifier{};
    public:
        /*!
         * @brief Get component from request
         * @return uuid string
         * */
        const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get VLAN port identifier
         * @return VLAN port identifier
         * */
        const string& get_port_identifier() const {
            return m_port_identifier;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    private:
        friend class json::GetPortVlansId;
        std::vector<std::string> m_vlans_id{};
    public:
        /*!
         * @brief Add vlan id
         * @param vlan_id Vlan id
         */
        void add_vlan_id(const std::string& vlan_id) {
            m_vlans_id.push_back(vlan_id);
        }

        ~Response();
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_NETWORK_GET_PORT_VLANS_ID_HPP */
