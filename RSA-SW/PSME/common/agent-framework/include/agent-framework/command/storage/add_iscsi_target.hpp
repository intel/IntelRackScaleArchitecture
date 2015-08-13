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
 * @file command/storage/get_target_info.hpp
 * @brief Generic storage AddISCSITarget command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_STORAGE_ADD_ISCSI_TARGET_HPP
#define AGENT_FRAMEWORK_COMMAND_STORAGE_ADD_ISCSI_TARGET_HPP

#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/command/command.hpp"
#include <vector>

namespace agent_framework {
namespace command {
namespace storage {

/* Forward declaration */
namespace json { class AddISCSITarget; }

/*! Generic storage command AddISCSITarget */
class AddISCSITarget : public Command {
    public:
        /*! Tag string for identify agent */
        static constexpr const char AGENT[] = "Storage";

        /*! Tag string for identify AddISCSITarget command */
        static constexpr const char TAG[] = "addiSCSITarget";

        /*! Lun, drive object mapper */
        class TargetLun {
        public:
            /*!
             * @brief Construct lun to drive mapper object
             * @param drive Drive UUID
             * @param lun Lun number
             */
             TargetLun(const std::string& drive, std::uint64_t lun) :
                 m_drive{drive}, m_lun{lun} {}

            /*!
             * @brief Get drive uuid
             * @return Drive UUID
             */
            const std::string& get_drive() const {
                return m_drive;
            }

            /*!
             * @brief Get lun number
             * @return Lun number
             */
            std::uint64_t get_lun() const {
                return m_lun;
            }

        private:
            std::string m_drive{};
            std::uint64_t m_lun{};
        };
        using TargetLunVec = std::vector<TargetLun>;

        /*! Argument request to execute */
        class Request : public Argument {
            friend class json::AddISCSITarget;
            std::string m_initiator_iqn{};
            std::string m_target_iqn{};
            TargetLunVec m_target_luns{};
            generic::OEMData m_oem_data{};
        public:
            /*!
             * @brief Get target luns
             * @return Target luns
             */
            const TargetLunVec& get_target_luns() const {
                return m_target_luns;
            }

            /*!
             * @brief Get target iqn
             * @return Target iqn
             */
            const std::string& get_target_iqn() const {
                return m_target_iqn;
            }

            /*!
             * @brief Get initiator iqn
             * @return Initiator iqn
             */
            const std::string& get_initiator_iqn() const {
                return m_initiator_iqn;
            }

            /*!
             * @brief Get OEM data
             * @return OEM data object
             */
            const generic::OEMData& get_oem_data() const {
                return m_oem_data;
            }

            ~Request();
        };

        /*! Argument response from execute */
        class Response : public Argument {
            friend class json::AddISCSITarget;
            std::string m_target{};
            std::string m_target_address{};
            std::uint32_t m_target_port{};
            generic::OEMData m_oem_data{};

        public:
            /*!
             * @brief Set target uuid
             * @param target Target uuid
             */
            void set_target(const std::string& target) {
                m_target = target;
            }

            /*!
             * @brief Set target address
             * @param target_address Target address
             */
            void set_target_address(const std::string& target_address) {
                m_target_address = target_address;
            }

            /*!
             * @brief Set target port
             * @param port Target port
             */
            void set_target_port(const std::uint32_t port) {
                m_target_port = port;
            }

            /*!
             * @brief Set oem data
             * @param oem_data OEMData object
             */
            void set_oem_data(const generic::OEMData& oem_data) {
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
        virtual ~AddISCSITarget();

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

#endif /* AGENT_FRAMEWORK_COMMAND_STORAGE_ADD_ISCSI_TARGET_HPP */
