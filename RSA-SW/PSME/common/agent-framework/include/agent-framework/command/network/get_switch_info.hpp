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
 * @file command/network/get_switch_info.hpp
 * @brief Generic network GetSwitchInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_NETWORK_GET_SWITCH_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_NETWORK_GET_SWITCH_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/fru_info.hpp"
#include "agent-framework/module/oem_data.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace network {

/* Forward declaration */
namespace json { class GetSwitchInfo; }

using Status = agent_framework::generic::Status;
using FRUInfo = agent_framework::generic::FruInfo;
using OEMData = agent_framework::generic::OEMData;

/*! Generic network command GetSwitchInfoInfo */
class GetSwitchInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Network";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getSwitchInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetSwitchInfo();
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
        friend class json::GetSwitchInfo;
        std::string m_component{};
    public:
        /*!
         * @brief Get component from request
         * @return uuid string
         * */
        const string& get_component() const {
            return m_component;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    private:
        friend class json::GetSwitchInfo;
        Status m_status{};
        std::string m_technology{};
        std::string m_mac_address{};
        FRUInfo m_fru_info{};
        OEMData m_oem_data{};
    public:

        /*!
         * @brief Set status from request
         * @param status switch status
         */
        void set_status(const Status& status) {
            m_status = status;
        }

        /*!
         * @brief Set technology from request
         * @param technology switch technology
         */
        void set_technology(const std::string& technology) {
            m_technology = technology;
        }

        /*!
         * @brief Set mac address from request
         * @param mac_address switch mac address
         */
        void set_mac_address(const std::string& mac_address) {
            m_mac_address = mac_address;
        }

        /*!
         * @brief Set FRUInfo structure
         * @param fru_info switch FRUInfo
         */
        void set_fru_info(const FRUInfo& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * @brief Set OEM data
         * @param oem switch OEM data
         */
        void set_oem(const OEMData& oem) {
            m_oem_data = oem;
        }

        ~Response();
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_NETWORK_GET_SWITCH_INFO_HPP */
