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
 * @brief Generic storage GetTargetInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_STORAGE_GET_TARGET_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_STORAGE_GET_TARGET_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/target.hpp"

namespace agent_framework {
namespace command {
namespace storage {

/* Forward declaration */
namespace json { class GetTargetInfo; }

/*! Generic storage command GetTargetInfo */
class GetTargetInfo : public Command {
public:
    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Storage";

    /*! Tag string for identify CreateTarget command */
    static constexpr const char TAG[] = "getiSCSITargetInfo";

    /*! Argument request to execute */
    class Request : public Argument {
        friend class json::GetTargetInfo;
        std::string m_target{};
    public:

        /*!
         * @brief Get target uuid
         * @return Target
         * */
        const std::string& get_target() const {
            return m_target;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
        friend class json::GetTargetInfo;
        agent_framework::generic::Target m_target_data{};
    public:

        /*!
         * @brief Set target data
         * @param target Target data object
         */
        void set_target_data(const agent_framework::generic::Target& target) {
            m_target_data = target;
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
    virtual ~GetTargetInfo();

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

#endif /* AGENT_FRAMEWORK_COMMAND_STORAGE_GET_TARGET_INFO_HPP */
