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
 * @file command/compute/get_drive_info.hpp
 * @brief Generic compute GetDriveInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_DRIVE_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_DRIVE_INFO_HPP

#include "agent-framework/module/status.hpp"
#include "agent-framework/module/fru_info.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/command/command.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace compute {

/* Forward declaration */
namespace json { class GetDriveInfo; }

using std::string;
using std::uint32_t;
using std::shared_ptr;

using Status = agent_framework::generic::Status;
using FruInfo = agent_framework::generic::FruInfo;
using OEMData = agent_framework::generic::OEMData;

/*! Generic storage command GetDriveInfo */
class GetDriveInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Compute";

    /*! Tag string for identify GetDriveInfo command */
    static constexpr const char TAG[] = "getDriveInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetDriveInfo();
protected:
    /*!
     * @brief Execute command with given command arguments
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
        friend class json::GetDriveInfo;
        string m_component{};
        uint32_t m_drive{};
        uint32_t m_controller{};
    public:
        /*!
         * @brief Get component UUID from request
         * @return UUID of component.
         * */
        const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get drive number
         * @return Drive number
         * */
        uint32_t get_drive() const {
            return m_drive;
        }

        /*!
         * @brief Get controller
         * @return Controller number
         * */
        uint32_t get_controller() const { return m_controller; }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:

        /*! Default constructor */
        Response() { }

        /*!
         * @brief Set drive interface for response
         * @param[in] interface Drive interface (PCIe, SAS, SATA)
         * */
        void set_interface(const string& interface) {
            m_interface = interface;
        }

        /*!
         * @brief Set drive type for response
         * @param[in] type Drive type (HDD, SSD, NVMe)
         * */
        void set_type(const string& type) {
            m_type = type;
        }

        /*!
         * @brief Set drive capacity for response
         * @param[in] capacity_gb Drive capacity in GB
         * */
        void set_capacity_gb(uint32_t capacity_gb) {
            m_capacity_gb = capacity_gb;
        }

        /*!
         * @brief Set drive RPM for response
         * @param[in] rpm Drive RPM
         * */
        void set_rpm(uint32_t rpm) {
            m_rpm = rpm;
        }

        /*!
         * @brief Set FruInfo for response
         * @param[in] fru_info FruInfo
         * */
        void set_fru_info(const FruInfo& fru_info) {
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
        void set_oem(shared_ptr<OEMData> const oem) {
            m_oem = oem;
        }

        ~Response();
    private:
        friend class json::GetDriveInfo;
        string m_interface{};
        string m_type{};
        uint32_t m_capacity_gb{};
        uint32_t m_rpm{};
        FruInfo m_fru_info{};
        Status m_status{};
        shared_ptr<const OEMData> m_oem{new OEMData};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_DRIVE_INFO_HPP */
