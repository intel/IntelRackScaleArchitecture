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

#include "agent-framework/command/network/get_port_vlans_id.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "api/switch_port_info.hpp"
#include "api/netlink/switch_port_info.hpp"
#include "network_config.hpp"

using std::runtime_error;

using namespace agent::network;
using namespace agent_framework::command;
using namespace agent_framework::generic;

namespace agent {
namespace network {
namespace hw {
namespace fm10000 {

using namespace agent_framework;
using namespace api::netlink;

class GetPortVlansId : public command::network::GetPortVlansId {

public:
    GetPortVlansId() { }

    using agent_framework::command::network::GetPortVlansId::execute;

    void execute(const Request& request, Response& response) {
#ifdef NL3_FOUND
        try {
            SwitchPortInfo::PortAttributeValue value{};
            SwitchPortInfo port_info(request.get_component(),
                                     request.get_port_identifier());
            port_info.get_switch_port_attribute(SwitchPortInfo::VLANLIST,
                                                value);
            const auto& vlans = port_info.get_vlan_list();
            for (const auto& vlan_id : vlans) {
                response.add_vlan_id(vlan_id);
            }
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm10000"), "Cannot get port vlan ids");
            log_debug(GET_LOGGER("fm10000"), error.what());
            throw exception::NotFound();
        }
#else
        (void) request;
        (void) response;
#endif
    }

    ~GetPortVlansId();
};

GetPortVlansId::~GetPortVlansId() { }

}
}
}
}

static Command::Register<agent::network::hw::fm10000::GetPortVlansId> g("fm10000");
