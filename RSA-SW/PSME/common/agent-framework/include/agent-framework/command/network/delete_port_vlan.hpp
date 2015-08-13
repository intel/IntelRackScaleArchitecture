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
 * @file command/network/delete_port_vlan.hpp
 * @brief Generic network DeletePortVlan command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_NETWORK_DELETE_PORT_VLAN_HPP
#define AGENT_FRAMEWORK_COMMAND_NETWORK_DELETE_PORT_VLAN_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/oem_data.hpp"
#include <string>

namespace agent_framework {
namespace command {
namespace network {

using OEMData = agent_framework::generic::OEMData;

/* Forward declaration */
namespace json { class DeletePortVlan; }

/*! Generic network command DeletePortVlan */
class DeletePortVlan : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Network";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "deletePortVlan";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~DeletePortVlan();
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
        friend class json::DeletePortVlan;
        string m_component{};
        string m_port_identifier{};
        string m_vlan_identifier{};
        OEMData m_oem{};
    public:
        /*!
         * @brief Set component from request
         * @return uuid string
         * */
        const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get port identifier
         * @return Port identifier
         * */
        const string& get_port_identifier() const {
            return m_port_identifier;
        }

        /*!
         * @brief Get vlan identifier
         * @return Vlan identifier
         * */
        const string& get_vlan_identifier() const {
            return m_vlan_identifier;
        }

        /*!
         * @brief Get OEM specific data
         * @return DeletePortVlan OEM specific data
         */
        const OEMData& get_oem() {
            return m_oem;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    private:
        friend class json::DeletePortVlan;
        OEMData m_oem_data{};

    public:
        /*!
         * @brief Set OEM data
         * @param[in] oem_data oem data
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

#endif /* AGENT_FRAMEWORK_COMMAND_NETWORK_DELETE_PORT_VLAN_HPP */
