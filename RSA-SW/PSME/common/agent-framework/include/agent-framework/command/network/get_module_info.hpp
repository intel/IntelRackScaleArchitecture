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
 * @file command/network/get_module_info.hpp
 * @brief Generic network GetModuleInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_NETWORK_GET_MODULE_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_NETWORK_GET_MODULE_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/fru_info.hpp"
#include "agent-framework/module/oem_data.hpp"

#include <string>
#include <vector>

namespace agent_framework {
namespace command {
namespace network {

/* Forward declaration */
namespace json { class GetModuleInfo; }

using std::string;

using Status = agent_framework::generic::Status;
using FRUInfo = agent_framework::generic::FruInfo;
using OEMData = agent_framework::generic::OEMData;

/*! Generic network command GetModuleInfo */
class GetModuleInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Network";

    /*! Tag string for identify GetModuleInfo command */
    static constexpr const char TAG[] = "getModuleInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetModuleInfo();
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
        friend class json::GetModuleInfo;
        string m_component{};
    public:
        /*!
         * @brief Get component id from request
         * @return Switch system uuid
         * */
        const string& get_component() const {
            return m_component;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    private:
        friend class json::GetModuleInfo;
        Status m_status{};
        unsigned int m_blade_count{};
        unsigned int m_switch_count{};
        std::uint32_t m_slot{};
        FRUInfo m_fru_info{};
        OEMData m_oem_data{};
    public:

        /*!
         * @brief Set status from request
         * @param status module status
         */
        void set_status(const Status& status) {
            m_status = status;
        }

        /*!
         * @brief Set blade count from request
         * @param blade_count module blade count
         */
        void set_blade_count(const unsigned int blade_count) {
            m_blade_count = blade_count;
        }

        /*!
         * @brief Set switch count
         * @param switch_count switch count
         */
        void set_switch_count(const unsigned int switch_count) {
            m_switch_count = switch_count;
        }

        /*!
         * @brief Set module slot
         * @param slot module slot
         */
        void set_slot(const std::uint32_t slot) {
            m_slot = slot;
        }

        /*!
         * @brief Set FRUInfo structure
         * @param fru_info module FRUInfo
         */
        void set_fru_info(const FRUInfo& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * @brief Set OEM data
         * @param oem module OEM data
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

#endif	/* AGENT_FRAMEWORK_COMMAND_SWITCH_GET_MODULE_INFO_HPP */
