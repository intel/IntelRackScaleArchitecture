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

#include "agent-framework/command/network/set_switch_port_attributes.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "api/switch_port_info.hpp"
#include "api/ies/switch_port_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

namespace agent {
namespace network {
namespace hw {
namespace fm6000 {

using namespace agent_framework;

class SetSwitchPortAttributes : public command::network::SetSwitchPortAttributes {

public:
    SetSwitchPortAttributes() { }

    using agent_framework::command::network::SetSwitchPortAttributes::execute;

    void execute(const Request& request, Response& response) {
#ifdef IES_FOUND
        const auto& uuid = request.get_component();
        const auto& port = request.get_port_identifier();

        string link_state = static_cast<string>(request.get_administrative_state());
        uint32_t link_speed = request.get_link_speed_gbps();
        uint32_t max_frame_size = request.get_frame_size();
        std::unique_ptr<api::ies::SwitchPortInfo> port_info{};

        try {
            port_info = std::unique_ptr<api::ies::SwitchPortInfo>
                                       (new api::ies::SwitchPortInfo(uuid, port));
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm6000"), "Invalid input parameters");
            log_debug(GET_LOGGER("fm6000"), error.what());
            throw exception::NotFound();
        }

        try{
            port_info->set_link_state(link_state);
            port_info->set_switch_port_attribute(api::ies::SwitchPortInfo::PortAttribute::LINKSTATE);
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm6000"), "Cannot set port link state");
            log_debug(GET_LOGGER("fm6000"), error.what());
            throw exception::InvalidArgument();
        }

        try{
            port_info->set_link_speed(link_speed);
            port_info->set_switch_port_attribute(api::ies::SwitchPortInfo::PortAttribute::LINKSPEEDGBPS);
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm6000"), "Cannot set port link speed");
            log_debug(GET_LOGGER("fm6000"), error.what());
            throw exception::InvalidArgument();
        }

        try{
            port_info->set_max_frame_size(max_frame_size);
            port_info->set_switch_port_attribute(api::ies::SwitchPortInfo::PortAttribute::FRAMESIZE);
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm6000"), "Cannot set port max frame size ");
            log_debug(GET_LOGGER("fm6000"), error.what());
            throw exception::InvalidArgument();
        }
#else
        (void) request;
#endif
        response.set_status("success");
    }

    ~SetSwitchPortAttributes();
};

SetSwitchPortAttributes::~SetSwitchPortAttributes() { }

}
}
}
}
static Command::Register<agent::network::hw::fm6000::SetSwitchPortAttributes> g("fm6000");
