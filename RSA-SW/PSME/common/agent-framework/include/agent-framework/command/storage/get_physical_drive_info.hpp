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
 * @file command/storage/get_physical_drive_info.hpp
 * @brief Generic storage GetPhysicalDriveInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_STORAGE_GET_PHYSICAL_DRIVE_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_STORAGE_GET_PHYSICAL_DRIVE_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/fru_info.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/status.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace storage {

/* Forward declaration */
namespace json { class GetPhysicalDriveInfo; }

using std::string;

using FruInfo = agent_framework::generic::FruInfo;
using OEMData = agent_framework::generic::OEMData;
using Status = agent_framework::generic::Status;
using OEMDataConstUniquePtr = OEMData::OEMDataConstUniquePtr;

/*! Generic storage command GetPhysicalDriveInfo */
class GetPhysicalDriveInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr char AGENT[] = "Storage";

    /*! Tag string for identify command */
    static constexpr char TAG[] = "getPhysicalDriveInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetPhysicalDriveInfo();
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
        friend class json::GetPhysicalDriveInfo;
        string m_drive{};
    public:
        /*!
         * @brief Get drive UUID
         * @return Drive UUID
         * */
        const string& get_drive() const {
            return m_drive;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:

        /*!
         * @brief Set drive interface (SAS, SATA, ...)
         * @param[in] interface Drive interface
         * */
        void set_interface(const string& interface) {
            m_interface = interface;
        }

        /*!
         * @brief Set drive capacity in GB
         * @param[in] capacity_gb Drive capacity in GB
         * */
        void set_capacity_gb(uint32_t capacity_gb) {
            m_capacity_gb = capacity_gb;
        }

        /*!
         * @brief Set drive type (HDD, SSD, ...)
         * @param[in] type Drive type
         * */
        void set_type(const string& type) {
            m_type = type;
        }

        /*!
         * @brief Set drive rotation per minute
         * @param[in] rpm Rotation per minute
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
        void set_oem(const OEMData* oem) {
            m_oem.reset(oem);
        }


        ~Response();
    private:
        friend class json::GetPhysicalDriveInfo;
        string m_interface{"SATA"};
        uint32_t m_capacity_gb{};
        string m_type{};
        uint32_t m_rpm{};
        FruInfo m_fru_info{};
        Status m_status{};
        OEMDataConstUniquePtr m_oem{new OEMData};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_STORAGE_GET_PHYSICAL_DRIVE_INFO_HPP */
