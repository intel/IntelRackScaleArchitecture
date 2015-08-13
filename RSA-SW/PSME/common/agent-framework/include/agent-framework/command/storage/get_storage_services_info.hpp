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
 * @file command/storage/get_storage_services_info.hpp
 * @brief Generic storage GetStorageServicesInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_STORAGE_GET_STORAGE_SERVICES_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_STORAGE_GET_STORAGE_SERVICES_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/collection.hpp"

#include <vector>

namespace agent_framework {
namespace command {
namespace storage {

/* Forward declaration */
namespace json { class GetStorageServicesInfo; }

using Status = generic::Status;
using Collection = generic::Collection;
using OEMData = generic::OEMData;
using OEMDataConstUniquePtr = generic::OEMData::OEMDataConstUniquePtr;

/*! Generic storage command GetStorageServicesInfo */
class GetStorageServicesInfo : public Command {
public:
    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Storage";

    /*! Tag string for identify GetStorageServicesInfo command */
    static constexpr const char TAG[] = "getStorageServicesInfo";

    /*! Argument request to execute */
    class Request : public Argument {
        friend class json::GetStorageServicesInfo;
        string m_services{};
    public:
        /*!
         * @brief Get storage services instance UUID
         * @return Storage services instance UUID
         * */
        const string& get_services() const {
            return m_services;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:
        /*! Default class constructor */
        Response() {}

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
         * @brief Add collection to response
         * @param[in] collection Collection
         * */
        void add_collection(const Collection::CollectionEntry& collection) {
            m_collections.add_collection_entry(collection);
        }
    private:
        Status m_status{};
        OEMDataConstUniquePtr m_oem{new OEMData};
        Collection m_collections{};
        friend class json::GetStorageServicesInfo;
    public:
        ~Response();
    };
    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetStorageServicesInfo();

protected:

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   in          Input request argument
     * @param[out]  out         Output response argument
     * */
    void execute(const Argument& in, Argument& out) override final {
        execute(static_cast<const Request&>(in), static_cast<Response&>(out));
    }
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_STORAGE_GET_STORAGE_SERVICES_INFO_HPP */
