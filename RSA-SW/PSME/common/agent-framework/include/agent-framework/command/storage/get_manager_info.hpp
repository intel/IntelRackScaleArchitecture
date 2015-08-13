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
 * @file command/storage/get_manager_info.hpp
 * @brief Generic storage GetManagerInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_STORAGE_GET_MANAGER_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_STORAGE_GET_MANAGER_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/oem_data.hpp"

#include <string>
#include <vector>

namespace agent_framework {
namespace command {
namespace storage {

/* Forward declaration */
namespace json { class GetManagerInfo; }

using std::string;

/*! Generic storage command GetManagerInfo */
class GetManagerInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Storage";

    /*! Tag string for identify GetManagerInfo command */
    static constexpr const char TAG[] = "getManagerInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetManagerInfo();
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
        friend class json::GetManagerInfo;
        string m_component{};
    public:
        /*!
         * @brief Get component id from request
         * @return Component UUID
         * */
        const string& get_component() const {
            return m_component;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    private:
        friend class json::GetManagerInfo;

        class NetworkService {
        public:
            string m_name;
            uint32_t m_port;
            bool m_enabled;
        };

        class SerialConsole {
        public:
            string m_parity{};
            string m_signal_type{};
            string m_flow_control{};
            string m_pin_out{};
            uint32_t m_bitrate{};
            uint32_t m_data_bits{};
            uint32_t m_stop_bits{};
            bool m_enabled{};
        };

        using NetworkServices = std::vector<NetworkService>;

        string m_firmware_version {};
        string m_ipv4address {};
        NetworkServices m_network_services {};
        SerialConsole m_serial_console {};
        agent_framework::generic::Status m_status {};
        agent_framework::generic::OEMData m_oem_data {};
    public:
        /*!
         * @brief Set firmware version for response based on component field
         * @param[in] firmware_version string
         * */
        void set_firmware_version(const string& firmware_version) {
            m_firmware_version = firmware_version;
        }
        /*!
         * @brief Set IPv4 for response based on component field
         * @param[in] ipv4address string e.g. "127.0.0.1"
         * */
        void set_ipv4_address(const string& ipv4address) {
            m_ipv4address = ipv4address;
        }
        /*!
         * @brief Set manager state
         * @param[in] state state string
         * */
        void set_status_state(const string& state) {
            m_status.set_state(state);
        }
        /*!
         * @brief Set manager health
         * @param[in] health health string
         * */
        void set_status_health(const string& health) {
            m_status.set_health(health);
        }
        /*!
         * @brief Set network service name for response based on component field
         * @param[in] service instance of available network service
         * */
        void add_network_service(const NetworkService& service) {
            m_network_services.push_back(service);
        }
        /*!
         * @brief Set serial console type for response based on component field
         * @param[in] type console type e.g. "Rs232"
         * */
        void set_serial_console_type(const string& type) {
            m_serial_console.m_signal_type = type;
        }
        /*!
         * @brief Set serial console bitrate for response based on component field
         * @param[in] bitrate console bitrate
         * */
        void set_serial_console_bitrate(const uint32_t bitrate) {
            m_serial_console.m_bitrate = bitrate;
        }
        /*!
         * @brief Set serial console parity for response based on component field
         * @param[in] parity console parity e.g. "None"
         * */
        void set_serial_console_parity(const string& parity) {
            m_serial_console.m_parity = parity;
        }
        /*!
         * @brief Set serial console data bits for response based on component field
         * @param[in] databits console data bits number
         * */
        void set_serial_console_databits(const uint32_t databits) {
            m_serial_console.m_data_bits = databits;
        }
        /*!
         * @brief Set serial console stop bits for response based on component field
         * @param[in] stopbits console stop bits number
         * */
        void set_serial_console_stopbits(const uint32_t stopbits) {
            m_serial_console.m_stop_bits = stopbits;
        }
        /*!
         * @brief Set serial console flow control for response based on component field
         * @param[in] flowControl console flow control e.g. "None"
         * */
        void set_serial_console_flow_control(const string& flowControl) {
            m_serial_console.m_flow_control = flowControl;
        }
        /*!
         * @brief Set serial console pinout for response based on component field
         * @param[in] pinout console pinout e.g. "Cisco"
         * */
        void set_serial_console_pinout(const string& pinout) {
            m_serial_console.m_pin_out = pinout;
        }
        /*!
         * @brief Set serial console enabled for response based on component field
         * @param[in] enabled console
         * */
        void set_serial_console_enabled(const bool enabled) {
            m_serial_console.m_enabled = enabled;
        }

        ~Response();
    };
};

}
}
}

#endif	/* AGENT_FRAMEWORK_COMMAND_STORAGE_GET_MANAGER_INFO_HPP */
