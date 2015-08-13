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
#include "logger/logger_factory.hpp"
#include "hw/fm6000/network_controller_manager.hpp"
#include "hw/fm6000/network_controller.hpp"

#include <netlink/genl/genl.h>

#ifdef IES_FOUND
#include "switch_vlan.hpp"

using namespace agent::network::api::ies;
using namespace agent::network::hw::fm6000;

SwitchInfo::SwitchInfo(const std::string& uuid) {
    auto network_controller = NetworkControllerManager::get_network_controller();
    NetworkController::SwitchId switch_id = network_controller->get_switch_id(uuid);
    set_is_enabled(network_controller->get_is_initialized());
    set_switch_id(uint16_t(switch_id));
}

SwitchInfo::~SwitchInfo() {}

void SwitchInfo::read_switch_info() {
    read_mgmt_mac_address();
}

void SwitchInfo::read_switch_port_list() {
    // Get switch port list
    auto network_controller = NetworkControllerManager::get_network_controller();
    set_port_list(network_controller->get_port_list());
}
#endif /* IES_FOUND */
