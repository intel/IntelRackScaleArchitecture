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
 * @file command/network/get_neighbor_switches_id.hpp
 * @brief Generic network GetNeighborSwitchesId command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_NETWORK_GET_NEIGHBOR_SWITCHES_ID_HPP
#define AGENT_FRAMEWORK_COMMAND_NETWORK_GET_NEIGHBOR_SWITCHES_ID_HPP

#include "agent-framework/command/command.hpp"

#include <vector>
#include <string>

namespace agent_framework {
namespace command {
namespace network {

/* Forward declaration */
namespace json { class GetNeighborSwitchesId; }

/*! Generic network command GetNeighborSwitchesId */
class GetNeighborSwitchesId : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Network";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getNeighborSwitchesId";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetNeighborSwitchesId();
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
        friend class json::GetNeighborSwitchesId;
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
    private:
        friend class json::GetNeighborSwitchesId;
        std::vector<std::string> m_switches_id{};
    public:
        /*!
         * @brief Add switch id
         * @param switch_id Switch id
         */
        void add_switch_id(const std::string& switch_id) {
            m_switches_id.push_back(switch_id);
        }

        ~Response();
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_NETWORK_GET_NEIGHBOR_SWITCHES_ID_HPP */
