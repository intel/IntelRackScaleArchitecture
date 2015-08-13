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
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_STORAGE_JSON_SET_COMPONENT_ATTRIBUTES_HPP
#define AGENT_FRAMEWORK_COMMAND_STORAGE_JSON_SET_COMPONENT_ATTRIBUTES_HPP

#include "agent-framework/command/command_json.hpp"
#include "agent-framework/command/storage/set_component_attributes.hpp"

namespace agent_framework {
namespace command{
namespace storage {
namespace json {

using agent_framework::command::CommandJson;

class SetComponentAttributes : public CommandJson {
public:
    static constexpr const char* AGENT = storage::SetComponentAttributes::AGENT;
    static constexpr const char* TAG = storage::SetComponentAttributes::TAG;

    SetComponentAttributes();

    void method(const Json::Value& params, Json::Value& result) final override;

    void notification(const Json::Value& params) final override;

private:
    void m_add_attribute(storage::SetComponentAttributes::Request& request,
                         const Json::Value& attribute_json);
};

}
}
}
}

#endif /*! AGENT_FRAMEWORK_COMMAND_STORAGE_JSON_SET_COMPONENT_ATTRIBUTES_HPP */
