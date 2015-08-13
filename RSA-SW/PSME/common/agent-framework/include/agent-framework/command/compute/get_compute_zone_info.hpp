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
 * @file command/compute/get_compute_zone_info.hpp
 * @brief Generic compute GetComputeZoneInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COMPUTE_ZONE_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COMPUTE_ZONE_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/collection.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace compute {

/* Forward declaration */
namespace json { class GetComputeZoneInfo; }

/*! Generic compute command GetComputeZoneInfo */
class GetComputeZoneInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Compute";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getComputeZoneInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetComputeZoneInfo();
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
        friend class json::GetComputeZoneInfo;
        std::string m_zone{};
    public:
        /*!
         * @brief Get zone from request
         * @return uuid string
         * */
        const std::string& get_zone() const {
            return m_zone;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:
        /*!
         * @brief Set status from request
         * @param status compute zone status
         */
        void set_status(const generic::Status& status) {
            m_status = status;
        }

        /*!
         * @brief Sets collection
         * @param[in] collection Collection reference
         * */
        void set_collection(const generic::Collection& collection) {
            m_collection.set_collection(collection.get_collection());
        }

        /*!
         * @brief Set OEM data
         * @param oem compute zone OEM data
         */
        void set_oem(const generic::OEMData& oem) {
            m_oem_data = oem;
        }

        ~Response();

    private:
        friend class json::GetComputeZoneInfo;
        generic::Status m_status{};
        generic::Collection m_collection{};
        generic::OEMData m_oem_data{};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COMPUTE_ZONE_INFO_HPP */
