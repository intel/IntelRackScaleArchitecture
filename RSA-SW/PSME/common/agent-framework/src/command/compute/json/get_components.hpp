/*!
 * @section LICENSE
 *
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
 * @section DESCRIPTION
 *
 * @file command/compute/json/get_component_info.hpp
 *
 * @brief JSON command get component info interface
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_JSON_GET_COMPONENTS_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_JSON_GET_COMPONENTS_HPP

#include "agent-framework/command/command_json.hpp"
#include "agent-framework/command/compute/get_components.hpp"

namespace agent_framework {
namespace command {
namespace compute {
namespace json {

using agent_framework::command::CommandJson;

/*! JSON compute command class */
class GetComponents : public CommandJson {
public:
    /*! Agent name tag */
    static constexpr const char* AGENT = compute::GetComponents::AGENT;

    /*! Command name tag */
    static constexpr const char* TAG = compute::GetComponents::TAG;

    /*!
     * @brief Create JSON command
     * */
    GetComponents();

    /*!
     * @brief JSON RPC method
     *
     * @param[in] params JSON RPC params request
     * @param[out] result JSON RPC result response
     * */
    void method(const Json::Value& params, Json::Value& result) final override;

    /*!
     * @brief JSON RPC notification
     *
     * @param[in] params JSON RPC params request
     * */
    void notification(const Json::Value& params) final override;

private:
    Json::Value m_add_component(
        compute::GetComponents::Response::Component & component);

    Json::Value m_add_components(
        std::vector<compute::GetComponents::Response::Component> & components);

};

} /* namespace json */
} /* namespace compute */
} /* namespace command */
} /* namespace agent_framework */

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_JSON_GET_COMPONENTS_HPP */
