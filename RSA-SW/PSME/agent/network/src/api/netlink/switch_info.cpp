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
 * @file switch_info.cpp
 *
 * @brief Switch parameters
 * */
#include "switch_info.hpp"

#include "hw/fm10000/network_controller_manager.hpp"
#include "hw/fm10000/network_controller.hpp"

#include "message.hpp"
#include "sysfs.hpp"

#include <net/if.h>

using namespace agent::network::api::netlink;
using namespace agent::network::hw::fm10000;

SwitchInfo::SwitchInfo(const std::string& uuid) {
    static const int CPU_PORT = 0;
    auto controller = NetworkControllerManager::get_network_controller();
    NetworkController::SwitchId switch_id = controller->get_switch_id(uuid);
    const auto& ifname = Message::create_port_identifier(
                                Message::Switch(switch_id), CPU_PORT);
    set_is_enabled(0 != if_nametoindex(ifname.c_str()));
    set_switch_id(uint16_t(switch_id));
}

SwitchInfo::~SwitchInfo() {}

void SwitchInfo::read_switch_info() {
    read_mgmt_mac_address();
}

void SwitchInfo::read_switch_port_list() {
    SysFs sysfs;
    set_port_list(sysfs.get_port_list());
}
