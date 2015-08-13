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
#include "api/netlink/switch_port_info.hpp"
#include "network_config.hpp"

using std::runtime_error;

using namespace agent_framework::command;
using namespace agent_framework::generic;

namespace agent {
namespace network {
namespace hw {
namespace fm10000 {

using namespace agent_framework;

using namespace api::netlink;

class SetSwitchPortAttributes : public command::network::SetSwitchPortAttributes {

public:
    SetSwitchPortAttributes() { }

    using agent_framework::command::network::SetSwitchPortAttributes::execute;

    void execute(const Request& request, Response& response) {
#ifdef NL3_FOUND
        uint32_t link_speed_gbps = request.get_link_speed_gbps();
        const string& link_state = request.get_administrative_state();
        uint32_t frame_size = request.get_frame_size();
        SwitchPortInfo::PortAttributeValue auto_sense{};
        auto_sense.set(bool(request.get_auto_sense()));

        try {
            SwitchPortInfo port_info(request.get_component(),
                                     request.get_port_identifier());

            /* set link speed */
            if (link_speed_gbps) {
                SwitchPortInfo::PortAttributeValue value{};
                value.set(link_speed_gbps);
                port_info.set_switch_port_attribute(SwitchPortInfo::LINKSPEEDGBPS,
                                                    value);
            }
            /* set max frame size */
            if (frame_size) {
                SwitchPortInfo::PortAttributeValue value{};
                value.set(frame_size);
                port_info.set_switch_port_attribute(SwitchPortInfo::FRAMESIZE,
                                                    value);
            }
            /* set auto-sense */
            port_info.set_switch_port_attribute(SwitchPortInfo::AUTOSENSE,
                                                auto_sense);
            /* set link state */
            port_info.set_link_state(link_state);
            if (port_info.link_state_is_valid()) {
                /* Given state is valid to be set */
                SwitchPortInfo::PortAttributeValue value{};
                value.set(link_state);
                port_info.set_switch_port_attribute(SwitchPortInfo::LINKSTATE,
                                                    value);
            }
            else {
                throw runtime_error("Provided link state is not valid");
            }
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm10000"), "Cannot set port information");
            log_debug(GET_LOGGER("fm10000"), error.what());
            throw exception::NotFound();
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
static Command::Register<agent::network::hw::fm10000::SetSwitchPortAttributes> g("fm10000");
