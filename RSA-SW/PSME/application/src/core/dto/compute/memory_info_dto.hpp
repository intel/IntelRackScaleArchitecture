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

#ifndef COMPUTE_MEMORY_INFO_DTO_HPP
#define COMPUTE_MEMORY_INFO_DTO_HPP

#include "core/dto/status_dto.hpp"
#include "core/dto/fru_info_dto.hpp"
#include "core/dto/oem_data_dto.hpp"
#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Compute namespace */
namespace compute {

/*! Memory information request object */
class MemoryInfoDTO {
public:
    /*! Copy constructor */
    MemoryInfoDTO(const MemoryInfoDTO &) = default;

    /*! Assigment constructor */
    MemoryInfoDTO& operator=(const MemoryInfoDTO &) = default;

    /*! Default constructor */
    MemoryInfoDTO() {}

    /*! Memory information DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::uint32_t m_socket{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets component
         *
         * @param component Component
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets component
         *
         * @return Component
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * Sets memory socket number
         *
         * @param socket Memory socket
         * */
        void set_socket(std::uint32_t socket) {
            m_socket = socket;
        }

        /*!
         * Gets memory socket
         *
         * @return Memory socket
         * */
        std::uint32_t get_socket() const {
            return m_socket;
        }

        virtual ~Request();
    };

    /*! Memory information DTO response */
    class Response : public ResponseDTO {
        std::string m_socket{};
        std::string m_type{"DDR3"};
        std::uint32_t m_size_gb{};
        std::uint32_t m_speed_mhz{};
        double m_voltage_volt{};
        std::uint32_t m_data_width_bits{};
        std::uint32_t m_total_width_bits{};
        std::string m_form_factor{"Unknown"};
        StatusDTO::Response m_status{};
        FRUInfoDTO::Response m_fru_info{};
        OEMDataDTO::Response m_oem_data{};
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
         * Sets socket
         *
         * @param socket Socket
         * */
        void set_socket(const std::string& socket) {
            m_socket = socket;
        }

        /*!
         * Gets socket
         *
         * @return Socket
         * */
        const std::string& get_socket() const {
            return m_socket;
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
         * Gets memory type
         *
         * @return Memory type
         * */
        const std::string& get_type() const {
            return m_type;
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
         * Gets memory size in GB
         *
         * @return Memory size
         */
        std::uint32_t get_size_gb() const {
            return m_size_gb;
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
         * Gets memory speed in Mhz
         *
         * @return Memory speed
         */
        std::uint32_t get_speed_mhz() const {
            return m_speed_mhz;
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
         * Gets memory voltage
         *
         * @return Memory voltage
         * */
        double get_voltage_volt() const {
            return m_voltage_volt;
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
         * Gets data width in bits
         *
         * @return Data width in bits
         * */
        std::uint32_t get_data_width_bits() const {
            return m_data_width_bits;
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
         * Gets total width in bits
         *
         * @return Total width in bits
         * */
        std::uint32_t get_total_width_bits() const {
            return m_total_width_bits;
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
         * Gets form factor
         *
         * @return Form factor
         * */
        const std::string& get_form_factor() const {
            return m_form_factor;
        }

        /*!
         * Sets status
         *
         * @param status Status
         * */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * Gets status
         *
         * @return Status
         * */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * Sets FRU info
         *
         * @param fru_info FRU info
         * */
        void set_fru_info(const FRUInfoDTO::Response& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * Gets FRU info
         *
         * @return FRU info
         * */
        const FRUInfoDTO::Response& get_fru_info() const {
            return m_fru_info;
        }

        /*!
         * Sets OEM specific data
         *
         * @param oem_data OEM specific data
         * */
        void set_oem_data(const OEMDataDTO::Response& oem_data) {
            m_oem_data = oem_data;
        }

        /*!
         * Gets OEM specific data
         *
         * @return OEM specific data
         * */
        const OEMDataDTO::Response& get_oem_data() const {
            return m_oem_data;
        }

        /*! Virtual destructor */
        virtual ~Response();
    };

    ~MemoryInfoDTO();
};

}
}
}
}

#endif // COMPUTE_MEMORY_INFO_DTO_HPP
