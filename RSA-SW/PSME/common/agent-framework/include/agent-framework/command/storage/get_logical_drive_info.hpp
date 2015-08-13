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
 * @file command/storage/get_logical_drive_info.hpp
 * @brief Generic storage GetLogicalDriveInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_STORAGE_GET_LOGICAL_DRIVE_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_STORAGE_GET_LOGICAL_DRIVE_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/collection.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace storage {

/* Forward declaration */
namespace json { class GetLogicalDriveInfo; }

using std::string;

using OEMData = agent_framework::generic::OEMData;
using Status = agent_framework::generic::Status;
using OEMDataConstUniquePtr = OEMData::OEMDataConstUniquePtr;
using Collection = agent_framework::generic::Collection;

/*! Generic storage command GetLogicalDriveInfo */
class GetLogicalDriveInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr char AGENT[] = "Storage";

    /*! Tag string for identify command */
    static constexpr char TAG[] = "getLogicalDriveInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetLogicalDriveInfo();
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
        friend class json::GetLogicalDriveInfo;
        string m_drive{};
    public:

        /*!
         * @brief Get drive number
         * @return Drive number
         * */
        string get_drive() const {
            return m_drive;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:

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

        /*!
         * @brief Sets image
         * @param[in] image The image
         * */
        void set_image(const string& image) {
            m_image = image;
        }

        /*!
         * @brief Sets mode
         * @param[in] mode The mode
         * */
        void set_mode(const string& mode) {
            m_mode = mode;
        }

        /*!
         * @brief Sets bootable flag
         * @param[in] bootable Bootable flag
         * */
        void set_bootable(bool bootable) {
            m_bootable = bootable;
        }

        /*!
         * @brief Sets protected flag
         * @param[in] protected_ Protected flag
         * */
        void set_protected(bool protected_) {
            m_protected = protected_;
        }

        /*!
         * @brief Sets snapshot flag
         * @param[in] snapshot_ Snapshot flag
         * */
        void set_snapshot(bool snapshot_) {
            m_snapshot = snapshot_;
        }

        /*!
         * @brief Sets collection
         * @param[in] collection Collection reference
         * */
        void set_collection(const Collection& collection) {
            m_collection.set_collection(collection.get_collection());
        }

        /*!
         * @brief Sets the master
         * @param[in] master Master reference
         * */
        void set_master(const std::string& master) {
            m_master = master;
        }

        ~Response();
    private:
        friend class json::GetLogicalDriveInfo;
        Status m_status{};
        string m_type{};
        uint32_t m_capacity_gb{};
        string m_mode{};
        string m_image{};
        bool m_bootable{};
        bool m_protected{};
        bool m_snapshot{};
        string m_master{};
        Collection m_collection{};
        OEMDataConstUniquePtr m_oem{new OEMData};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_STORAGE_GET_LOGICAL_DRIVE_INFO_HPP */
