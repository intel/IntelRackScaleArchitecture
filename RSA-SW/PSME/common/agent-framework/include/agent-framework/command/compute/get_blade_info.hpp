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
 * @file command/compute/get_blade_info.hpp
 * @brief Generic compute GetBladeInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_BLADE_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_BLADE_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/fru_info.hpp"
#include "agent-framework/module/oem_data.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace compute {

using OEMData = agent_framework::generic::OEMData;
using FRUInfo = agent_framework::generic::FruInfo;
using Status = agent_framework::generic::Status;

/* Forward declaration */
namespace json { class GetBladeInfo; }

/*! Generic compute command GetBladeInfo */
class GetBladeInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Compute";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getBladeInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetBladeInfo();
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
        friend class json::GetBladeInfo;
        string m_component{};
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
    public:
        using BootSupportedVec = std::vector<std::string>;

        /*!
         * @brief Set status from request
         * @param status blade status
         */
        void set_status(const Status& status) {
            m_status = status;
        }

        /*!
         * @brief Set bios version from request
         * @param bios_version blade bios version
         */
        void set_bios_version(const std::string& bios_version) {
            m_bios_version = bios_version;
        }

        /*!
         * @brief Set boot override
         * @param boot_override Boot override value
         */
        void set_boot_override(const std::string& boot_override) {
            m_boot_override = boot_override;
        }

        /*!
         * @brief Set boot override target
         * @param boot_override_target Boot override target value
         */
        void
        set_boot_override_target(const std::string& boot_override_target) {
            m_boot_override_target = boot_override_target;
        }

        /*!
         * @brief Set boot override supported
         * @param boot_supported Boot override supported
         */
        void
        set_boot_override_supported(const BootSupportedVec& boot_supported) {
            m_boot_override_supported = boot_supported;
        }

        /*!
         * @brief Set processor count from request
         * @param processor_count blade processor count
         */
        void set_processor_count(const unsigned int processor_count) {
            m_processor_count = processor_count;
        }

        /*!
         * @brief Set dimm count from request
         * @param dimm_count blade dimm count
         */
        void set_dimm_count(const unsigned int dimm_count) {
            m_dimm_count = dimm_count;
        }

        /*!
         * @brief Set controller count from request
         * @param controller_count blade controller count
         */
        void set_controller_count(const unsigned int controller_count) {
            m_controller_count = controller_count;
        }

        /*!
         * @brief Set nic count from request
         * @param nic_count blade nic count
         */
        void set_nic_count(const unsigned int nic_count) {
            m_nic_count = nic_count;
        }

        /*!
         * @brief Set power state from request
         * @param power_state blade is powered on
         */
        void set_power_state(const string& power_state) {
            m_power_state = power_state;
        }

        /*!
         * @brief Set blade slot
         * @param slot blade slot
         */
        void set_slot(const std::uint32_t slot) {
            m_slot = slot;
        }

        /*!
         * @brief Set FRUInfo structure
         * @param fru_info blade FRUInfo
         */
        void set_fru_info(const FRUInfo& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * @brief Set OEM data
         * @param oem blade OEM data
         */
        void set_oem(const OEMData& oem) {
            m_oem_data = oem;
        }

        ~Response();

    private:
        friend class json::GetBladeInfo;
        Status m_status{};
        std::string m_bios_version{};
        std::string m_boot_override{};
        std::string m_boot_override_target{};
        BootSupportedVec m_boot_override_supported{"None", "Pxe", "Hdd"};
        unsigned int m_processor_count{};
        unsigned int m_dimm_count{};
        unsigned int m_controller_count{};
        unsigned int m_nic_count{};
        std::string m_power_state{"Off"};
        std::uint32_t m_slot{};
        FRUInfo m_fru_info{};
        OEMData m_oem_data{};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_BLADE_INFO_HPP */
