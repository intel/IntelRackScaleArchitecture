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
 * @file core/dto/request/processor_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef COMPUTE_PROCESSOR_INFO_DTO_HPP
#define COMPUTE_PROCESSOR_INFO_DTO_HPP

#include "core/dto/status_dto.hpp"
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

/*! Processor information request object */
class ProcessorInfoDTO {
public:
    /*! Copy constructor */
    ProcessorInfoDTO(const ProcessorInfoDTO &) = default;

    /*! Assigment constructor */
    ProcessorInfoDTO& operator=(const ProcessorInfoDTO &) = default;

    /*! Default constructor */
    ProcessorInfoDTO() {}

    /*! Processor information DTO request */
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
         * Sets component id
         *
         * @param component Component id
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets component id
         *
         * @return Component id
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * Sets processor socket
         *
         * @param socket Processor socket
         * */
        void set_socket(std::uint32_t socket) {
            m_socket = socket;
        }

        /*!
         * Gets processor socket
         *
         * @return Processor socket
         * */
        std::uint32_t get_socket() const {
            return m_socket;
        }

        virtual ~Request();
    };

    /*! Processor information DTO response */
    class Response : public ResponseDTO {
    public:

        /*! CPUID transfer object */
        class CPUID {
            std::string m_vendor_id{};
            std::uint32_t m_numeric_id{};
            std::uint32_t m_family{};
            std::uint32_t m_model{};
            std::uint32_t m_step{};
            std::uint32_t m_microcode_info{};
        public:
            /*! Copy constructor */
            CPUID(const CPUID &) = default;

            /*! Assigment constructor */
            CPUID& operator=(const CPUID &) = default;

            /*! Default constructor */
            CPUID() { }

            /*!
             * @brief Set vendor id
             * @param[in] vendor_id  Vendor id
             * */
            void set_vendor_id(const std::string& vendor_id) {
                m_vendor_id = vendor_id;
            }

            /*!
             * @brief Get vendor id
             * @return Vendor id
             * */
            const std::string& get_vendor_id() const {
                return m_vendor_id;
            }

            /*!
             * @brief Set numeric id
             * @param[in] numeric_id Numeric id
             * */
            void set_numeric_id(std::uint32_t numeric_id) {
                m_numeric_id = numeric_id;
            }

            /*!
             * @brief Get numeric id
             * @return Numeric id
             * */
            std::uint32_t get_numeric_id() const {
                return m_numeric_id;
            }

            /*!
             * @brief Set processor family
             * @param[in] family Processor family
             * */
            void set_family(std::uint32_t family) {
                m_family = family;
            }

            /*!
             * @brief Get processor family
             * @return Processor family
             * */
            std::uint32_t get_family() const {
                return m_family;
            }

            /*!
             * @brief Set processor model
             * @param[in] model Processor model
             * */
            void set_model(std::uint32_t model) {
                m_model = model;
            }

            /*!
             * @brief Get processor model
             * @return Processor model
             * */
            std::uint32_t get_model() const {
                return m_model;
            }

            /*!
             * @brief Set processor step
             * @param[in] step Processor step
             * */
            void set_step(std::uint32_t step) {
                m_step = step;
            }

            /*!
             * @brief Get processor step
             * @return Processor step
             * */
            std::uint32_t get_step() const {
                return m_step;
            }

            /*!
             * @brief Set microcode info
             * @param[in] microcode_info Microcode info
             * */
            void set_microcode_info(std::uint32_t microcode_info) {
                m_microcode_info = microcode_info;
            }

            /*!
             * @brief Get microcode info
             * @return Microcode info
             * */
            std::uint32_t get_microcode_info() const {
                return m_microcode_info;
            }

            /*!
             * Desarializes CPUID response object from JSON
             *
             * @param response Response JSON
             * */
            void to_object(const Json::Value& response);

            virtual ~CPUID();
        };

        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes processor information response object from JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Set socket ("CPU 1", ...)
         * @param[in] socket Socket
         * */
        void set_socket(const std::string& socket) {
            m_socket = socket;
        }

        /*!
         * @brief Get socket
         * @return Socket
         * */
        const std::string& get_socket() const {
            return m_socket;
        }

        /*!
         * @brief Set processor type (CPU, GPU, FPGA, ...)
         * @param[in] processor_type Processor type
         * */
        void set_processor_type(const std::string& processor_type) {
            m_processor_type = processor_type;
        }

        /*!
         * @brief Get processor type
         * @return Processor type
         * */
        const std::string& get_processor_type() const {
            return m_processor_type;
        }

        /*!
         * @brief Set processor architecture
         * @param[in] processor_architecture Processor architecture
         * */
        void set_processor_architecture(
            const std::string& processor_architecture) {
            m_processor_architecture = processor_architecture;
        }

        /*!
         * @brief Get processor architecture
         * @return Processor architecture
         * */
        const std::string& get_processor_architecture() const {
            return m_processor_architecture;
        }

        /*!
         * @brief Set instruction set
         * @param[in] instruction_set Instruction set
         * */
        void set_instruction_set(const std::string& instruction_set) {
            m_instruction_set = instruction_set;
        }

        /*!
         * @brief Get instruction set
         * @return Instruction set
         * */
        const std::string& get_instruction_set() const {
            return m_instruction_set;
        }

        /*!
         * @brief Set manufacturer
         * @param[in] manufacturer Manufacturer
         * */
        void set_manufacturer(const std::string& manufacturer) {
            m_manufacturer = manufacturer;
        }

        /*!
         * @brief Get manufacturer
         * @return Manufacturer
         * */
        const std::string& get_manufacturer() const {
            return m_manufacturer;
        }

        /*!
         * @brief Set model
         * @param[in] model Model
         * */
        void set_model(const std::string& model) {
            m_model = model;
        }

        /*!
         * @brief Get model
         * @return Model
         * */
        const std::string& get_model() const {
            return m_model;
        }

        /*!
         * @brief Set processor max speed in MHz
         * @param[in] max_speed_mhz Processor max speed in MHz
         * */
        void set_max_speed_mhz(std::uint32_t max_speed_mhz) {
            m_max_speed_mhz = max_speed_mhz;
        }

        /*!
         * @brief Get processor max speed
         * @return Processor max speed
         * */
        std::uint32_t get_max_speed_mhz() const {
            return m_max_speed_mhz;
        }

        /*!
         * @brief Set processor total cores
         * @param[in] total_cores Processor total cores
         * */
        void set_total_cores(std::uint32_t total_cores) {
            m_total_cores = total_cores;
        }

        /*!
         * @brief Get total cores
         * @return Total cores
         * */
        std::uint32_t get_total_cores() const {
            return m_total_cores;
        }

        /*!
         * @brief Set processor enabled cores
         * @param[in] enabled_cores Processor enabled cores
         * */
        void set_enabled_cores(std::uint32_t enabled_cores) {
            m_enabled_cores = enabled_cores;
        }

        /*!
         * @brief Get enabled cores
         * @return Enabled cores
         * */
        std::uint32_t get_enabled_cores() const {
            return m_enabled_cores;
        }

        /*!
         * @brief Set processor total threads
         * @param[in] total_threads Processor total threads
         * */
        void set_total_threads(std::uint32_t total_threads) {
            m_total_threads = total_threads;
        }

        /*!
         * @brief Get total threads
         * @return Total threads
         * */
        std::uint32_t get_total_threads() const {
            return m_total_threads;
        }

        /*!
         * @brief Set processor enabled threads
         * @param[in] enabled_threads Processor enabled threads
         * */
        void set_enabled_threads(std::uint32_t enabled_threads) {
            m_enabled_threads = enabled_threads;
        }

        /*!
         * @brief Get enabled threads
         * @return Enabled threads
         * */
        std::uint32_t get_enabled_threads() const {
            return m_enabled_threads;
        }

        /*!
         * Sets CPUID
         *
         * @param cpuid CPUID
         * */
        void set_cpuid(const CPUID& cpuid) {
            m_cpuid = cpuid;
        }

        /*!
         * Gets CPUID
         *
         * @return CPUID
         * */
        const CPUID& get_cpuid() const {
            return m_cpuid;
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


        virtual ~Response();

    private:
        std::string m_socket{};
        std::string m_processor_type{};
        std::string m_processor_architecture{};
        std::string m_instruction_set{"x86:64"};
        std::string m_manufacturer{};
        std::string m_model{};
        std::uint32_t m_max_speed_mhz{};
        std::uint32_t m_total_cores{};
        std::uint32_t m_enabled_cores{};
        std::uint32_t m_total_threads{};
        std::uint32_t m_enabled_threads{};
        StatusDTO::Response m_status{};
        OEMDataDTO::Response m_oem_data{};
        CPUID m_cpuid{};
    };

    ~ProcessorInfoDTO();
};

}
}
}
}

#endif /* COMPUTE_PROCESSOR_INFO_DTO_HPP */
