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
 * @file command/compute/get_processor_info.hpp
 * @brief Generic compute GetProcessorInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_PROCESSOR_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_PROCESSOR_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/status.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace compute {

/* Forward declaration */
namespace json { class GetProcessorInfo; }

using std::string;

using OEMData = agent_framework::generic::OEMData;
using Status = agent_framework::generic::Status;
using OEMDataConstUniquePtr = OEMData::OEMDataConstUniquePtr;

/*! Generic compute command GetProcessorInfo */
class GetProcessorInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr char AGENT[] = "Compute";

    /*! Tag string for identify command */
    static constexpr char TAG[] = "getProcessorInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetProcessorInfo();
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
        friend class json::GetProcessorInfo;
        string m_component{};
        unsigned int m_socket{};
    public:
        /*!
         * @brief Get component id from request
         * @return Computer system id string (UUID)
         * */
        inline const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get socket number
         * @return Socket number
         * */
        inline unsigned int get_socket() const {
            return m_socket;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:

        /*! Class representing CPUID attribute
         *  for GetProcessorInfo Response */
        class CPUID {
            string m_vendor_id{};
            uint32_t m_numeric_id{};
            uint32_t m_family{};
            uint32_t m_model{};
            uint32_t m_step{};
            uint32_t m_microcode_info{};
            friend class json::GetProcessorInfo;
        public:
           /*!
            * @brief CPUID constructor
            * */
            CPUID() { }

            /*!
            * @brief CPUID copy constructor
            * */
            CPUID(const CPUID&) = default;

           /*!
            * @brief CPUID assignment operator
            * */
            CPUID& operator=(const CPUID&) = default;

            /*!
            * @brief Set vendor id for response
            * @param[in] vendor_id  Vendor id
            * */
            void set_vendor_id(const string& vendor_id) {
                m_vendor_id = vendor_id;
            }

            /*!
             * @brief Set numeric id for response
             * @param[in] numeric_id Numeric id
             * */
            void set_numeric_id(uint32_t numeric_id) {
                m_numeric_id = numeric_id;
            }

            /*!
             * @brief Set processor family for response
             * @param[in] family Processor family
             * */
            void set_family(uint32_t family) {
                m_family = family;
            }

            /*!
             * @brief Set processor model for response
             * @param[in] model Processor model
             * */
            void set_model(uint32_t model) {
                m_model = model;
            }

            /*!
             * @brief Set processor step for response
             * @param[in] step Processor step
             * */
            void set_step(uint32_t step) {
                m_step = step;
            }

            /*!
             * @brief Set microcode info for response
             * @param[in] microcode_info Microcode info
             * */
            void set_microcode_info(uint32_t microcode_info) {
                m_microcode_info = microcode_info;
            }

            ~CPUID();
        };

        /*!
         * @brief Set socket ("CPU 1", ...)
         * @param[in] socket Socket
         * */
        void set_socket(const string& socket) {
            m_socket = socket;
        }

        /*!
         * @brief Set processor type (CPU, GPU, FPGA, ...)
         * @param[in] processor_type Processor type
         * */
        void set_processor_type(const string& processor_type) {
            m_processor_type = processor_type;
        }

        /*!
         * @brief Set processor architecture
         * @param[in] processor_architecture Processor architecture
         * */
        void set_processor_architecture(const string& processor_architecture) {
            m_processor_architecture = processor_architecture;
        }

        /*!
         * @brief Set instruction set
         * @param[in] instruction_set Instruction set
         * */
        void set_instruction_set(const string& instruction_set) {
            m_instruction_set = instruction_set;
        }

        /*!
         * @brief Set manufacturer
         * @param[in] manufacturer Manufacturer
         * */
        void set_manufacturer(const string& manufacturer) {
            m_manufacturer = manufacturer;
        }

        /*!
         * @brief Set model
         * @param[in] model Model
         * */
        void set_model(const string& model) {
            m_model = model;
        }

        /*!
         * @brief Set processor max speed in MHz
         * @param[in] max_speed_mhz Processor max speed in MHz
         * */
        void set_max_speed_mhz(uint32_t max_speed_mhz) {
            m_max_speed_mhz = max_speed_mhz;
        }

        /*!
         * @brief Set processor total cores
         * @param[in] total_cores Processor total cores
         * */
        void set_total_cores(uint32_t total_cores) {
            m_total_cores = total_cores;
        }

        /*!
         * @brief Set processor enabled cores
         * @param[in] enabled_cores Processor enabled cores
         * */
        void set_enabled_cores(uint32_t enabled_cores) {
            m_enabled_cores = enabled_cores;
        }

        /*!
         * @brief Set processor total threads
         * @param[in] total_threads Processor total threads
         * */
        void set_total_threads(uint32_t total_threads) {
            m_total_threads = total_threads;
        }

        /*!
         * @brief Set processor enabled threads
         * @param[in] enabled_threads Processor enabled threads
         * */
        void set_enabled_threads(uint32_t enabled_threads) {
            m_enabled_threads = enabled_threads;
        }

        /*!
         * @brief Set Status for response
         * @param[in] status Status
         * */
        void set_status(const Status& status) {
            m_status = status;
        }

        /*!
         * @brief Set CPUID for response
         * @param cpuid CPUID object
         */
        void set_cpuid(const CPUID& cpuid) {
            m_cpuid = cpuid;
        }

        /*!
         * @brief Set OEM specific data for response
         * @param[in] oem OEM specific data
         * */
        void set_oem(const OEMData* oem) {
            m_oem.reset(oem);
        }


        ~Response();
    private:
        friend class json::GetProcessorInfo;
        string m_socket{};
        string m_processor_type{};
        string m_processor_architecture{};
        string m_instruction_set{};
        string m_manufacturer{};
        string m_model{};
        uint32_t m_max_speed_mhz{};
        uint32_t m_total_cores{};
        uint32_t m_enabled_cores{};
        uint32_t m_total_threads{};
        uint32_t m_enabled_threads{};
        Status m_status{};
        CPUID m_cpuid{};
        OEMDataConstUniquePtr m_oem{new OEMData};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_PROCESSOR_INFO_HPP */
