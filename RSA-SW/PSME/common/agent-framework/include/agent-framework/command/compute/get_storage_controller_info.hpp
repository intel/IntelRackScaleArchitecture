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
 * @file command/compute/get_storage_controller_info.hpp
 * @brief Generic compute GetStorageControllerInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_STORAGE_CONTROLLER_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_STORAGE_CONTROLLER_INFO_HPP

#include "agent-framework/command/command.hpp"

#include "agent-framework/module/module.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/fru_info.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/module/module_manager.hpp"


namespace agent_framework {
namespace command {
namespace compute {

using std::string;
using std::shared_ptr;

using Status = agent_framework::generic::Status;
using OEMData = agent_framework::generic::OEMData;
using FruInfo = agent_framework::generic::FruInfo;

namespace json { class GetStorageControllerInfo; }

/*! Generic storage command GetStorageControllerInfo */
class GetStorageControllerInfo : public Command {
    public:
        /*! Tag string for identify agent */
        static constexpr const char AGENT[] = "Compute";

        /*! Tag string for identify CreateTarget command */
        static constexpr const char TAG[] = "getStorageControllerInfo";

        /*! Argument request to execute */
        class Request : public Argument {
            uint32_t m_controller{};
            string m_component{};
            friend class json::GetStorageControllerInfo;
        public:
            uint32_t get_controller() const { return m_controller; }

            const string& get_component() const { return m_component; }

            ~Request();
        };

        /*! Argument response from execute */
        class Response : public Argument {
            Status m_status{};
            FruInfo m_fru_info{};
            string m_interface{};
            uint32_t m_drive_count{};
            shared_ptr<const OEMData> m_oem_data{new OEMData};
            friend class json::GetStorageControllerInfo;
        public:
            /*!
             * @brief Sets status of outgoing response
             * @param[in] status storage controller status
             * */
            void set_status(const Status& status) { m_status = status; }

            /*!
             * @brief Sets OEM data of outgoing response
             * @param[in] oem_data OEM data associated with specific storage
             * controller
             * */
            void set_oem_data(shared_ptr<OEMData> oem_data) { m_oem_data = oem_data; }

            /*!
             * @brief Sets drive count number of outgoing response
             * @param[in] drive_count Amount of drives associated with storage
             * controller
             * */
            void set_drive_count(const uint32_t& drive_count) { m_drive_count = drive_count; }

            /*!
             * @brief Sets FRU information of outgoing response
             * @param[in] fru_info FRU information associated with storage
             * controller
             * */
            void set_fru_info(const FruInfo& fru_info) { m_fru_info = fru_info; }

            /*!
             * @brief Sets interface of outgoing response
             * @param[in] interface Interface associated with storage controller
             * */
            void set_interface(const string& interface) { m_interface = interface; }

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
        virtual ~GetStorageControllerInfo();

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

#endif /*! AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_STORAGE_CONTROLLER_INFO_HPP*/
