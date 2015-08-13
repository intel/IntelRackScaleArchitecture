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

#include "agent-framework/command/compute/get_network_interface_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! GetNetworkInterfaceInfo implementation */
class GetNetworkInterfaceInfo : public compute::GetNetworkInterfaceInfo {
public:
    GetNetworkInterfaceInfo() {}

    using compute::GetNetworkInterfaceInfo::execute;

    void execute(const Request&, Response& response) {
        response.set_frame_size(1520);
        response.set_link_technology("Ethernet");
        response.set_speed_gbps(1);
        response.set_autosense(true);
        response.set_mac_address("AA:BB:CC:DD:EE:FF");
        response.set_ipv4_address({"10.0.2.2",
                                   "255.255.255.0",
                                   "DHCP",
                                   "10.0.2.1"});
        response.set_ipv6_address({"fe80::1ec1:deff:fe6f:1c37",
                                   16,
                                   "DHCP",
                                   "Perferred"});
        response.set_neighbor_info({"123e4567-e89b-12d3-a456-426655440000",
                                    "19"});
        response.set_vlan_enable(true);
        response.set_vlan_count(1);
        response.set_status({"Enabled", "OK"});
    }

    ~GetNetworkInterfaceInfo();
};

GetNetworkInterfaceInfo::~GetNetworkInterfaceInfo() {}

static Command::Register<GetNetworkInterfaceInfo> g("Intel");
