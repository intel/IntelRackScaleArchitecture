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
 * @file command/storage/delete_iscsi_target.hpp
 * @brief Generic storage DeleteISCSITarget command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_STORAGE_DELETE_ISCSI_TARGET_HPP
#define AGENT_FRAMEWORK_COMMAND_STORAGE_DELETE_ISCSI_TARGET_HPP

#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/command/command.hpp"

namespace agent_framework {
namespace command {
namespace storage {

/* Forward declaration */
namespace json { class DeleteISCSITarget; }

/*! Generic storage command DeleteISCSITarget */
class DeleteISCSITarget : public Command {
    public:
        /*! Tag string for identify agent*/
        static constexpr const char AGENT[] = "Storage";

        /*! Tag string for identify DeleteISCSITarget command */
        static constexpr const char TAG[] = "deleteiSCSITarget";

        /*! Argument request to execute */
        class Request : public Argument {
            friend class json::DeleteISCSITarget;
            std::string m_target{};
        public:
            /*!
             * @brief Get target uuid
             * @return Target uuid
             */
            const std::string& get_target() const {
                return m_target;
            }

            ~Request();
        };

        /*! Argument response from execute */
        class Response : public Argument {
            friend class json::DeleteISCSITarget;
            generic::OEMData m_oem_data{};
        public:

            /*!
             * @brief Set OEM data
             * @param oem_data OEM data object
             */
            void set_oem(const generic::OEMData& oem_data) {
                m_oem_data = oem_data;
            }

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
        virtual ~DeleteISCSITarget();

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

#endif /* AGENT_FRAMEWORK_COMMAND_STORAGE_DELETE_TARGET_HPP */
