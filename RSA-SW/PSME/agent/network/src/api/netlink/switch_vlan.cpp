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
 * @file switch_vlan.cpp
 *
 * @brief Netlink Vlan functionality.
 * */

#include "switch_vlan.hpp"
#include "socket.hpp"
#include "vlan_message.hpp"

#include "hw/fm10000/network_controller_manager.hpp"
#include "hw/fm10000/network_controller.hpp"

using namespace agent::network::api::netlink;
using namespace agent::network::hw::fm10000;

SwitchVlan::SwitchVlan(const std::string& uuid, VlanId vlan_id) {
    auto controller = NetworkControllerManager::get_network_controller();
    NetworkController::SwitchId switch_id = controller->get_switch_id(uuid);
    set_switch_id(SwitchId(switch_id));
    set_vlan_id(VlanId(vlan_id));
}

void SwitchVlan::add_port(const PortIdentifier& port, bool tagged) {
    Socket socket;
    socket.connect();
    AddVlanPortMessage vlan_msg(0, get_vlan_id(), tagged);
    vlan_msg.set_port(port); /* it will set port & switch id */
    socket.send_message(vlan_msg);
}

void SwitchVlan::remove_port(const PortIdentifier& port) {
    Socket socket;
    socket.connect();
    DelVlanPortMessage vlan_msg(0, get_vlan_id());
    vlan_msg.set_port(port); /* it will set port & switch id */
    socket.send_message(vlan_msg);
}

void SwitchVlan::get_info(const PortIdentifier& port, VlanPortInfo& info) {
    Socket socket;
    socket.connect();
    InfoVlanPortMessage vlan_msg;
    socket.send_message(vlan_msg);
    socket.receive_message(vlan_msg);
    info.set_tagged(vlan_msg.is_tagged(port, get_vlan_id()));
}
