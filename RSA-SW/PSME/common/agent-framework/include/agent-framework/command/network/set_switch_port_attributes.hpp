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
 * @file command/network/set_switch_port_attributes.hpp
 * @brief Generic network SetSwitchPortAttributes command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_NETWORK_SET_SWITCH_PORT_ATTRIBUTES_HPP
#define AGENT_FRAMEWORK_COMMAND_NETWORK_SET_SWITCH_PORT_ATTRIBUTES_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"

#include <vector>
#include <string>

namespace agent_framework {
namespace command {
namespace network {

using std::vector;

/* Forward declaration */
namespace json { class SetSwitchPortAttributes; }

using Status = agent_framework::generic::Status;

/*! Generic network command SetSwitchPortAttributes */
class SetSwitchPortAttributes : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Network";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "setSwitchPortAttributes";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~SetSwitchPortAttributes();
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
        friend class json::SetSwitchPortAttributes;
        std::string m_component{};
        std::string m_port_identifier{};
        std::uint32_t m_link_speed_gbps{};
        std::string m_administrative_state{};
        std::uint32_t m_frame_size{};
        bool m_auto_sense{};
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

        /*!
         * @brief Get link speed in Gbps from request
         * @return Link speed in Gbps
         * */
        uint32_t get_link_speed_gbps() const {
            return m_link_speed_gbps;
        }

        /*!
         * @brief Get administrative state from request
         * @return Administrative state
         * */
        const string& get_administrative_state() const {
            return m_administrative_state;
        }

        /*!
         * @brief Get frame size from request
         * @return Frame size
         * */
        uint32_t get_frame_size() const {
            return m_frame_size;
        }

        /*!
         * @brief Get auto sense from request
         * @return Auto sense
         */
        bool get_auto_sense() const {
            return m_auto_sense;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
        std::string m_status{};
        friend class json::SetSwitchPortAttributes;
    public:
        /*!
         * @brief Set status
         * @param status status
         */
        void set_status(const std::string& status) {
            m_status = status;
        }

        ~Response();
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_NETWORK_SET_SWITCH_PORT_ATTRIBUTES_HPP */
