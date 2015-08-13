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
 * @file command/compute/get_memory_info.hpp
 * @brief Generic compute GetMemoryInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_MEMORY_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_MEMORY_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/fru_info.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/status.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace compute {

/* Forward declaration */
namespace json { class GetMemoryInfo; }

using std::string;
using std::uint32_t;

using OEMData = agent_framework::generic::OEMData;
using FRUInfo = agent_framework::generic::FruInfo;
using Status = agent_framework::generic::Status;
using OEMDataConstUniquePtr = OEMData::OEMDataConstUniquePtr;

/*! Generic compute command GetMemoryInfo */
class GetMemoryInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Compute";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getMemoryInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetMemoryInfo();
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
        friend class json::GetMemoryInfo;
        string m_component{};
        unsigned int m_socket{};
    public:
        /*!
         * @brief Get component from request
         * @return uuid string
         * */
        inline const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get memory socket number
         * @return Memory socket number
         * */
        inline unsigned int get_socket() const {
            return m_socket;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    private:
        friend class json::GetMemoryInfo;
        std::string m_socket{};
        std::string m_type{};
        std::uint32_t m_size_gb{};
        std::uint32_t m_speed_mhz{};
        double m_voltage_volt{};
        std::uint32_t m_data_width_bits{};
        std::uint32_t m_total_width_bits{};
        std::string m_form_factor{};
        FRUInfo m_fru_info{};
        Status m_status{};
        OEMDataConstUniquePtr m_oem{new OEMData};
    public:

        /*!
         * Sets socket
         *
         * @param socket Socket
         * */
        void set_socket(const std::string& socket) {
            m_socket = socket;
        }

        /*!
         * Sets memory type
         *
         * @param type Memory type
         * */
        void set_type(const std::string& type) {
            m_type = type;
        }

        /*!
         * Sets memory size in GB
         *
         * @param size_gb Memory size
         */
        void set_size_gb(std::uint32_t size_gb) {
            m_size_gb = size_gb;
        }

        /*!
         * Sets memory speed in Mhz
         *
         * @param speed Memory speed
         */
        void set_speed_mhz(std::uint32_t speed) {
            m_speed_mhz = speed;
        }

        /*!
         * Sets memory voltage
         *
         * @param voltage_volt Memory voltage
         * */
        void set_voltage_volt(double voltage_volt) {
            m_voltage_volt = voltage_volt;
        }

        /*!
         * Sets data width in bits
         *
         * @param data_width_bits Data width in bits
         * */
        void set_data_width_bits(std::uint32_t data_width_bits) {
            m_data_width_bits = data_width_bits;
        }

        /*!
         * Sets total width in bits
         *
         * @param total_width_bits Total width in bits
         * */
        void set_total_width_bits(std::uint32_t total_width_bits) {
            m_total_width_bits = total_width_bits;
        }

        /*!
         * Sets form factor
         *
         * @param form_factor Form factor (DIMM, SODIMM, ...)
         * */
        void set_form_factor(const std::string& form_factor) {
            m_form_factor = form_factor;
        }

        /*!
         * @brief Set FruInfo for response
         * @param[in] fru_info FruInfo
         * */
        void set_fru_info(const FRUInfo& fru_info) {
            m_fru_info = fru_info;
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
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_MEMORY_INFO_HPP */
