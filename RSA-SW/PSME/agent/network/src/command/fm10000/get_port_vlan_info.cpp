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

#include "agent-framework/command/network/get_port_vlan_info.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "api/netlink/switch_vlan.hpp"
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

class GetPortVlanInfo : public command::network::GetPortVlanInfo {

public:
    GetPortVlanInfo() { }

    using agent_framework::command::network::GetPortVlanInfo::execute;

    void execute(const Request& request, Response& response) {
#ifdef NL3_FOUND
        const auto& uuid = request.get_component();
        auto vlan_id = std::stoi(request.get_vlan_identifier());
        SwitchVlan::VlanPortInfo info{};
        try {
            SwitchVlan vlan(uuid, SwitchVlan::VlanId(vlan_id));
            vlan.get_info(request.get_port_identifier(), info);
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm10000"), "Cannot get vlan port info");
            log_debug(GET_LOGGER("fm10000"), error.what());
            throw exception::NotFound();
        }
        response.set_tagged(info.get_tagged());
        response.set_vlan_id(uint32_t(vlan_id));
#else
        (void) request;
#endif
        response.set_status({"Enabled", "OK"});
        response.set_vlan_enable(true);
        response.set_ipv4address({"10.0.2.10",
                                "255.255.255.0",
                                "DHCP",
                                "10.0.2.1"});
        response.set_ipv6address({  "fe80:1ec1:deff:fe6f:1c37",
                                    16,
                                    "DHCP",
                                    "Preferred"});
        response.set_oem({});
    }

    ~GetPortVlanInfo();
};

GetPortVlanInfo::~GetPortVlanInfo() { }

}
}
}
}

static
Command::Register<agent::network::hw::fm10000::GetPortVlanInfo> g("fm10000");
