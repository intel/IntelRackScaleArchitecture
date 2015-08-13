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
 * @file command/compute/get_chassis_info.hpp
 * @brief Generic chassis GetChassisInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_CHASSIS_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_CHASSIS_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/fru_info.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/collection.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace compute {

using OEMData = agent_framework::generic::OEMData;
using FRUInfo = agent_framework::generic::FruInfo;
using Status = agent_framework::generic::Status;
using Collection = generic::Collection;

/* Forward declaration */
namespace json { class GetChassisInfo; }

/*! Generic chassis command GetChassisInfo */
class GetChassisInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Compute";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getChassisInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetChassisInfo();
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
        friend class json::GetChassisInfo;
        string m_chassis{};
    public:
        /*!
         * @brief Get chassis UUID from request
         * @return uuid string
         * */
        const string& get_chassis() const {
            return m_chassis;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:
        /*!
         * @brief Set status from request
         * @param status chassis status
         */
        void set_status(const Status& status) {
            m_status = status;
        }

        /*!
         * @brief Set chassis type
         * @param type Chassis type
         */
        void set_type(const std::string& type) {
            m_type = type;
        }

        /*!
         * @brief Set size
         * @param size Size
         */
        void set_size(std::uint32_t size) {
            m_size = size;
        }

        /*!
         * @brief Set location offset
         * @param location_offset Location offset
         */
        void set_location_offset(std::uint32_t location_offset) {
            m_location_offset = location_offset;
        }

        /*!
         * @brief Set FRUInfo structure
         * @param fru_info chassis FRUInfo
         */
        void set_fru_info(const FRUInfo& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * @brief Set OEM data
         * @param oem chassis OEM data
         */
        void set_oem(const OEMData& oem) {
            m_oem_data = oem;
        }

        /*!
         * @brief Add collection to response
         * @param[in] collection Collection
         * */
        void add_collection(const Collection::CollectionEntry& collection) {
            m_collections.add_collection_entry(collection);
        }

        ~Response();

    private:
        friend class json::GetChassisInfo;
        Status m_status{};
        std::string m_type{};
        std::uint32_t m_size{};
        std::uint32_t m_location_offset{};
        FRUInfo m_fru_info{};
        OEMData m_oem_data{};
        Collection m_collections{};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_CHASSIS_INFO_HPP */
