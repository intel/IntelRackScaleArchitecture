/*!
 * @section LICENSE
 *
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
 * @section DESCRIPTION
 *
 * */

#ifndef MANAGER_INFO_DTO_HPP
#define MANAGER_INFO_DTO_HPP

#include "request_dto.hpp"
#include "response_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {

/*! Network Service data object */
class NetworkService {
public:
    /*!
     * @brief Sets network service name
     *
     * @param name network service name
     */
    void set_name(const std::string& name) {
        m_name = name;
    }

    /*!
     * @brief Gets network service name
     *
     * @return network service name
     */
    const std::string& get_name() const {
        return m_name;
    }

    /*!
     * @brief Sets network service port
     *
     * @param port network service port
     *
     */
    void set_port(const unsigned int port) {
        m_port = port;
    }

    /*!
     * @brief Gets network service port
     *
     * @return network service port
     */
    unsigned int get_port() const {
        return m_port;
    }

    /*!
     * @brief Sets network service enable
     *
     * @param enabled network service enable
     */
    void set_enabled(const bool enabled) {
        m_enabled = enabled;
    }

    /*!
     * @brief Gets network service enable
     *
     * @return network service enable
     */
    bool get_enabled() const {
        return m_enabled;
    }

private:
    std::string m_name{};
    unsigned int m_port{};
    bool m_enabled{};
};

class SerialConsole {
public:

    /*! Copy constructor */
    SerialConsole(const SerialConsole &) = default;

    /*! Assigment constructor */
    SerialConsole& operator=(const SerialConsole &) = default;

    SerialConsole(){}

    /*!
     * @brief Sets signal type
     *
     * @param signal_type Signal Type
     */
    void set_signal_type(const std::string& signal_type) {
        m_signal_type = signal_type;
    }

    /*!
     * @brief Gets signal type
     *
     * @return signal type
     */
    const std::string& get_signal_type() const {
        return m_signal_type;
    }

    /*!
     * @brief Sets bitrate
     *
     * @param bitrate Bitrate
     */
    void set_bitrate(const unsigned int bitrate) {
        m_bitrate = bitrate;
    }

    /*!
     * @brief Gets bitrate
     *
     * @return bitrate
     */
    unsigned int get_bitrate() const {
        return m_bitrate;
    }

    /*!
     * @brief Sets parity
     *
     * @param parity Parity
     */
    void set_parity(const std::string& parity) {
        m_parity = parity;
    }

    /*!
     * @brief Gets parity
     *
     * @return parity
     */
    const std::string& get_parity() const {
        return m_parity;
    }

    /*!
     * @brief Sets data bits
     *
     * @param data_bits Data Bits
     */
    void set_data_bits(const unsigned int data_bits) {
        m_data_bits = data_bits;
    }

    /*!
     * @brief Gets data bits
     *
     * @return data bits
     */
    unsigned int get_data_bits() const {
        return m_data_bits;
    }

    /*!
     * @brief Sets stop bits
     *
     * @param stop_bits Stop Bits
     */
    void set_stop_bits(const unsigned int stop_bits) {
        m_stop_bits = stop_bits;
    }

    /*!
     * @brief Gets stop bits
     *
     * @return stop bits
     */
    unsigned int get_stop_bits() const {
        return m_stop_bits;
    }

    /*!
     * @brief Sets flow control
     *
     * @param flow_control Flow Control
     */
    void set_flow_control(const std::string& flow_control) {
        m_flow_control = flow_control;
    }

    /*!
     * @brief Gets flow contorl
     *
     * @return flow control
     */
    const std::string& get_flow_control() const {
        return m_flow_control;
    }

    /*!
     * @brief Sets pin out
     *
     * @param pin_out PinOut
     */
    void set_pin_out(const std::string& pin_out) {
        m_pin_out = pin_out;
    }

    /*!
     * @brief Gets pin out
     *
     * @return pin out
     */
    const std::string& get_pin_out() const {
        return m_pin_out;
    }

    /*!
     * @brief Sets enabled
     *
     * @param enabled Enabled
     */
    void set_enabled(const bool enabled) {
        m_enabled = enabled;
    }

    /*!
     * @brief Gets enabled
     *
     * @return enabled
     */
    bool get_enabled() const {
        return m_enabled;
    }

    ~SerialConsole();
private:
    unsigned int m_bitrate{};
    unsigned int m_data_bits{};
    unsigned int m_stop_bits{};
    std::string m_signal_type{};
    std::string m_parity{};
    std::string m_flow_control{};
    std::string m_pin_out{};
    bool m_enabled{};
};

using NetworkServices = std::vector<NetworkService>;

/*! Manager information request object */
class ManagerInfoDTO {
public:
    /*! Copy constructor */
    ManagerInfoDTO(const ManagerInfoDTO &) = default;

    /*! Assigment constructor */
    ManagerInfoDTO& operator=(const ManagerInfoDTO &) = default;

    /*! Default constructor */
    ManagerInfoDTO() {}

    /*! Manager information DTO request */
    class Request : public RequestDTO {
        std::string m_component_id{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets component id
         *
         * @param component_id Component id
         * */
        void set_component_id(const std::string& component_id) {
            m_component_id = component_id;
        }

        /*!
         * Gets component id
         *
         * @return Component id
         * */
        const std::string& get_component_id() const {
            return m_component_id;
        }

        virtual ~Request();
    };

    /*! Manager information DTO response */
    class Response : public ResponseDTO {
        std::string m_firmware_version{};
        std::string m_ipv4_address{};
        std::string m_ipv4_subnet_mask{};
        std::string m_ipv4_mac_address{};
        NetworkServices m_network_services{};
        SerialConsole m_serial_console{};

        NetworkService create_network_service(const Json::Value& response);
        NetworkServices create_network_services(const Json::Value& response);
        SerialConsole create_serial_console(const Json::Value& response);
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes memory information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Sets firmware version
         *
         * @param firmware_version Firmeware Version
         */
        void set_firmware_version(const std::string& firmware_version) {
            m_firmware_version = firmware_version;
        }

        /*!
         * @brief Gets firmware version
         *
         * @return firmware version
         */
        const std::string& get_firmware_version() const {
            return m_firmware_version;
        }

        /*!
         * @brief Sets IPv4 address
         *
         * @param ipv4_address IPv4 Address
         */
        void set_ipv4_address(const std::string& ipv4_address) {
            m_ipv4_address = ipv4_address;
        }

        /*!
         * @brief Gets IPv4 address
         *
         * @return ipv4 address
         */
        const std::string& get_ipv4_address() const {
            return m_ipv4_address;
        }

        /*!
         * @brief Sets Subnet Mask
         *
         * @param ipv4_subnet_mask IPv4 Subnet Mask
         */
        void set_ipv4_subnet_mask(const std::string& ipv4_subnet_mask) {
            m_ipv4_subnet_mask = ipv4_subnet_mask;
        }

        /*!
         * @brief Gets IPv4 Subnet Mask
         *
         * @return ipv4 subnet mask
         */
        const std::string& get_ipv4_subnet_mask() const {
            return m_ipv4_subnet_mask;
        }

        /*!
         * @brief Sets IPv4 MAC address
         *
         * @param ipv4_mac_address IPv4 MAC Address
         */
        void set_ipv4_mac_address(const std::string& ipv4_mac_address) {
            m_ipv4_mac_address = ipv4_mac_address;
        }

        /*!
         * @brief Gets IPv4 MAC address
         *
         * @return ipv4 MAC address
         */
        const std::string& get_ipv4_mac_address() const {
            return m_ipv4_mac_address;
        }

        /*!
         * @brief Sets network services
         *
         * @param network_services NetworkServices
         */
        void set_network_services(const NetworkServices& network_services) {
            m_network_services = network_services;
        }

        /*!
         * @brief Gets network services
         *
         * @return NetworkServices object
         */
        const NetworkServices& get_network_services() const {
            return m_network_services;
        }

        /*!
         * @brief Sets serial console
         *
         * @param serial_console SerialConsole
         */
        void set_serial_console(const SerialConsole& serial_console) {
            m_serial_console = serial_console;
        }

        /*!
         * @brief Gets serial console
         *
         * @return SerialConsole object
         */
        const SerialConsole& get_serial_console() const {
            return m_serial_console;
        }

        virtual ~Response();
    };

    ~ManagerInfoDTO();
};

}
}
}

#endif // MANAGER_INFO_DTO_HPP
