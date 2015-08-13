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
 * @file hw/network_controller.cpp
 *
 * @brief Interface for Network Controller. Implement this interface to
 * handle connections and sending/receiving commands.
 *
 * */

#include "network_controller.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "uuid++.hh"

using namespace agent_framework::generic;
using namespace agent::network::hw;
using std::string;
using std::to_string;
using std::runtime_error;

NetworkController::NetworkController() {
    for(auto& module : ModuleManager::get_modules()) {
        for(auto& submodule : module->get_submodules()) {
            if (submodule->get_type() == "RSASwitch") {
                m_switch_map[ submodule->get_name() ] = DEFAULT_SWITCH_ID;
                for(auto& port : submodule->get_switch_ports()) {
                    SwitchPortId port_id = static_cast<SwitchPortId>(port->get_id());
                    const std::string port_type = port->get_type();
                    m_switch_port_type_map[ port_id ] = port_type;
                }
                return;
            }
        }
    }
    throw runtime_error("RSASwitch module not found");
}

NetworkController::~NetworkController() {}

const std::string& NetworkController::get_switch_uuid(SwitchId switch_id) const {
    for(const auto& value : m_switch_map) {
        if (value.second == switch_id) {
            return value.first;
        }
    }
    throw runtime_error("Switch Id " + to_string(switch_id) + " not found");
}

NetworkController::SwitchId NetworkController::get_switch_id(UUID switch_uuid) const {
    const auto value = m_switch_map.find(switch_uuid);
    if (value != m_switch_map.end()) {
        return value->second;
    }
    throw runtime_error("Switch UUID " + switch_uuid + " not found");
}

const std::string NetworkController::get_switch_port_type(SwitchPortId port_id) const {
    const auto value = m_switch_port_type_map.find(port_id);
    if (value != m_switch_port_type_map.end()) {
        if (!value->second.empty()) {
           return value->second;
        }
    }
    return std::string{UNKNOWN_SWITCH_PORT_TYPE};
}
