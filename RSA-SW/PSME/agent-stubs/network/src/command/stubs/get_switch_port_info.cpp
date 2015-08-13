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

#include "agent-framework/command/network/get_switch_port_info.hpp"

using namespace agent_framework;
using namespace agent_framework::command;

class GetSwitchPortInfo : public command::network::GetSwitchPortInfo {
public:
    GetSwitchPortInfo() { }

    using agent_framework::command::network::GetSwitchPortInfo::execute;

    void execute(const Request&, Response& response) {
        response.set_status({"Enabled", "OK"});
        response.set_port_identifier("12");
        response.set_link_technology("Ethernet");
        response.set_link_speed(5);
        response.set_max_speed(10);
        response.set_administrative_state("Up");
        response.set_operational_state("Down");
        response.set_port_width(1);
        response.set_frame_size(1400);
        response.set_autosense(true);
        response.set_is_management_port(false);
        response.set_last_error_code(0);
        response.set_error_cleared(false);
        response.set_last_state_change_time("2015-02-23T14:44:00+00:00");
        response.set_mac_address("AA:BB:CC:DD:EE:FF");
        response.set_ipv4address({"10.0.2.10",
                                  "255.255.255.0",
                                  "DHCP",
                                  "10.0.2.1"});
        response.set_ipv6address({"fe80:1ec1:deff:fe6f:1c37",
                                  16,
                                  "DHCP",
                                  "Preferred"});
        response.set_neighbor_info({"123e4567-e89b-12d3-a456-426655440000",
                                    "19"});
        response.set_vlan_enable(true);
        response.set_oem({});
    }

    ~GetSwitchPortInfo();
};

GetSwitchPortInfo::~GetSwitchPortInfo() { }

static Command::Register<GetSwitchPortInfo> g("Stubs");
