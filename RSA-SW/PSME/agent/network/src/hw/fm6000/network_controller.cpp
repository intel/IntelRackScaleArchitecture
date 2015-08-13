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
 * @file hw/fm6000/network_controller.cpp
 *
 * @brief Implementation of network interface
 * */

#include "network_config.hpp"

#ifdef IES_FOUND
#include "network_controller.hpp"
#include "api/ies/switch_port_info.hpp"
#include "api/ies/switch_vlan.hpp"

#include "agent-framework/module/module_manager.hpp"

using namespace agent::network::hw::fm6000;
using namespace agent::network::api::ies;
using namespace agent_framework::generic;

static fm_semaphore g_seq_sem;

void NetworkController::initialize() {
    try {
        m_initialize_switch();

        m_initialize_ports();

        m_initialize_vlans();

        m_initialize_spanning_tree();

        m_set_ports_state();

        m_read_switch_info();

        m_is_initialized = true;
    }
    catch (std::exception& e) {
        log_error(GET_LOGGER("fm6000"), "Cannot initialize switch. " << e.what());
    }
}

fm_int NetworkController::get_ports_count() const {
    if (m_is_initialized) {
        /* Switch returns number of ports including P0 which is not used */
        return m_switch_info.numCardPorts - 1;
    }
    return 0;
}

const std::vector<std::string> NetworkController::get_port_list() const {
    std::vector<std::string> port_list{};
    int port{};

    for (port = 1; port <= get_ports_count(); ++port) {
        port_list.push_back(std::to_string(port));
    }

    /* return port list */
    return port_list;
}

void NetworkController::deinitialize() {
    m_is_initialized = false;
}

void NetworkController::m_initialize_switch() {
    fm_status retval = FM_FAIL;
    fm_timestamp wait_time = { 3, 0 };
    std::string text = "seq";

    log_debug(GET_LOGGER("fm6000"), "Initializing switch .....\n");

    // Initialize OS
    retval = fmOSInitialize();
    m_log_error_and_throw(retval, "OS initialization failed.");

    // Create the sequencing semaphore to wait until switch is inserted
    retval = fmCreateSemaphore(&text[0], FM_SEM_BINARY, &g_seq_sem, 0);
    m_log_error_and_throw(retval, "Creating switch semaphore failed.");

    // Initialize globals
    retval = fmInitialize(&m_switch_event_handler);
    m_log_error_and_throw(retval, "Switch events initialization failed.");

    // Wait for switch inserted
    retval = fmWaitSemaphore(&g_seq_sem, &wait_time);
    m_log_error_and_throw(retval, "Waiting for semaphore failed.");

    // Bring up switch
    retval = fmSetSwitchState(m_switch_id, 1);
    m_log_error_and_throw(retval, "Setting switch state up failed.");

    log_debug(GET_LOGGER("fm6000"), "Switch is successfully initialized.");
    log_debug(GET_LOGGER("fm6000"), "Switch UUID: " << get_switch_uuid(SwitchId(m_switch_id)));
}

void NetworkController::m_initialize_ports() {
    for(auto& module : ModuleManager::get_modules()) {
        for(auto& submodule : module->get_submodules()) {
            for(auto& port : submodule->get_switch_ports()) {
                fm_int port_id = static_cast<fm_int>(port->get_id());
                fm_uint64 port_eth_mode = static_cast<fm_uint64>(m_port_eth_mode_map[port->get_iface_mode()]);
                fm_uint64 port_autoneg_mode = static_cast<fm_uint64>(m_port_autoneg_map[port->get_autoneg_mode()]);

                // Set port ethernet interface mode
                if (port_eth_mode) {
                    SwitchPortInfo::write_switch_port_attribute(m_switch_id, port_id,
                        FM_PORT_ACTIVE_MAC, FM_PORT_LANE_ALL, FM_PORT_ETHERNET_INTERFACE_MODE, &port_eth_mode);
                    log_debug(GET_LOGGER("fm6000"), "Port " << fm_int(port_id)
                            << " ethernet interface mode set to" << port->get_iface_mode());
                }

                //Set port autonegotiation
                if (FM_PORT_AUTONEG_NONE != port_autoneg_mode) {
                    fm_uint64 port_autoneg_basepage = static_cast<fm_uint64>(m_port_basepage_map[port_autoneg_mode]);

                    SwitchPortInfo::write_switch_port_attribute(m_switch_id, port_id,
                        FM_PORT_ACTIVE_MAC, FM_PORT_LANE_ALL, FM_PORT_AUTONEG, &port_autoneg_mode);
                    SwitchPortInfo::write_switch_port_attribute(m_switch_id, port_id,
                        FM_PORT_ACTIVE_MAC, FM_PORT_LANE_ALL, FM_PORT_AUTONEG_BASEPAGE, &port_autoneg_basepage);

                    log_debug(GET_LOGGER("fm6000"), "Port " << fm_int(port_id)
                            << " autonegotiation mode set to" << port->get_autoneg_mode());
                 }
            }
        }
    }
}

void NetworkController::m_initialize_vlans() {
    uint16_t vlan_id = 1;

    log_debug(GET_LOGGER("fm6000"), "Initializing Vlans.");
    //@TODO Switch has no UUID assigned yet, so code below works only when
    //driver manages only one switch.
    for (auto& module : ModuleManager::get_modules()) {
        for (auto& submodule : module->get_submodules()) {
            //Create Vlans
            for (auto& vlan : submodule->get_vlans()) {
                bool is_created = false;
                vlan_id = static_cast<uint16_t>(vlan->get_id());
                SwitchVlan::create_switch_vlan(uint8_t(m_switch_id), vlan_id, &is_created);
                if (!is_created) {
                    throw runtime_error("Cannot create new Vlan");
                }
                log_debug(GET_LOGGER("fm6000"), "Vlan: " << vlan->get_id() << " successfully created.");
            }

            // Add ports to VLan
            for (auto& vlanport :submodule->get_vlanports()) {
                auto vlan = vlanport->get_vlan();
                auto port = vlanport->get_port();

                if (vlan && port) {
                    fm_int port_id = static_cast<fm_int>(port->get_id());
                    vlan_id = static_cast<uint16_t>(vlan->get_id());

                    if (vlanport->get_tagging()){
                        m_add_tagged_port_to_vlan(vlan_id, port_id);
                    }
                    else {
                        m_add_untagged_port_to_vlan(vlan_id, port_id);
                    }
                }

            }
        }
    }
}

void NetworkController::m_initialize_spanning_tree(void) {
    fm_uint32 value = FM_SPANNING_TREE_MULTIPLE;

    log_debug(GET_LOGGER("fm6000"), "Setting spanning tree algorithm.");

    fm_status retval = fmSetSwitchAttribute(m_switch_id, FM_SPANNING_TREE_MODE, &value);
    m_log_error_and_throw(retval, "Cannot set switch spanning tree mode.");

    log_debug(GET_LOGGER("fm6000"), "Configuring spannig tree port states.");
    for(auto& module : ModuleManager::get_modules()) {
        for(auto& submodule : module->get_submodules()) {
            for(auto& port : submodule->get_switch_ports()) {
                fm_int port_id = static_cast<fm_int>(port->get_id());
                retval = fmSetSpanningTreePortState(m_switch_id, 0,
                        port_id, FM_STP_STATE_FORWARDING);
                m_log_error_and_throw(retval, "Cannot set spanning tree port=" + std::to_string(port_id));
            }
        }
    }
}

void NetworkController::m_set_ports_state() {
    log_debug(GET_LOGGER("fm6000"), "Activating switch ports .....\n");

    for (auto& module : ModuleManager::get_modules()) {
        for (auto& submodule : module->get_submodules()) {
            for (auto& port : submodule->get_switch_ports()) {
                auto link_state = port->get_link_state();
                fm_int port_id = static_cast<fm_int>(port->get_id());
                try {
                    if (Port::LinkState::UP == link_state) {
                        log_debug(GET_LOGGER("fm6000"), "Setting port " << port_id << " up.");
                        SwitchPortInfo::write_switch_port_state(m_switch_id, port_id, FM_PORT_MODE_UP);
                    }
                    else {
                        log_debug(GET_LOGGER("fm6000"), "Setting port " << port_id << " down.");
                        SwitchPortInfo::write_switch_port_state(m_switch_id, port_id, FM_PORT_MODE_ADMIN_DOWN);
                    }
                }
                catch (const std::exception& e) {
                    log_warning(GET_LOGGER("fm6000"), "Cannot set port "
                            << port_id << " powerstate. " << e.what());
                }
            }
        }
    }
}

void NetworkController::m_read_switch_info() {
    log_debug(GET_LOGGER("fm6000"), "Getting switch info .....\n");

    /* Get switch information */
    fm_status retval = fmGetSwitchInfo(m_switch_id, &m_switch_info);
    m_log_error_and_throw(retval, "Cannot get switch information,");

    log_debug(GET_LOGGER("fm6000"), "Switch Family: " << fm_uint32(m_switch_info.switchFamily));
    log_debug(GET_LOGGER("fm6000"), "Switch Model: " << fm_uint32(m_switch_info.switchModel));
    log_debug(GET_LOGGER("fm6000"), "Switch Version: " << fm_uint32(m_switch_info.switchVersion));
    log_debug(GET_LOGGER("fm6000"), "Num Ports: " << fm_uint32(m_switch_info.numCardPorts));
    log_debug(GET_LOGGER("fm6000"), "Max VLANs: " << fm_uint32(m_switch_info.maxVLAN));
}

void NetworkController::m_switch_event_handler(fm_int event, fm_int switch_id, void *ptr) {
    fm_eventPort* portEvent = static_cast<fm_eventPort*>(ptr);

    switch (event) {
        case FM_EVENT_SWITCH_INSERTED:
            log_debug(GET_LOGGER("fm6000"), "Switch inserted.");
            if (0 == switch_id) {
                fmSignalSemaphore(&g_seq_sem);
            }
            break;
        case FM_EVENT_PORT:
            log_debug(GET_LOGGER("fm6000"), "Port="
                    << portEvent->port
                    << " operational state has been changed to "
                    << (portEvent->linkStatus ? "Up" : "Down"));
            break;
        default:
            log_warning(GET_LOGGER("fm6000"), "Unknown event");
            break;
    }
}

void NetworkController::m_add_tagged_port_to_vlan(uint16_t vlan_id, fm_int port_id) {
    fm_status retval = fmAddVlanPort(m_switch_id, vlan_id, port_id, TRUE);
    m_log_error_and_throw(retval, "Cannot add port=" + to_string(port_id) +
                " to vlan=" + to_string(static_cast<uint32_t>(vlan_id)));

    retval = fmSetVlanPortState(m_switch_id, vlan_id, port_id, FM_STP_STATE_FORWARDING);
    m_log_error_and_throw(retval, "Cannot set port=" + to_string(port_id) + " fowarding.");

    log_debug(GET_LOGGER("fm6000"), "Port " + to_string(port_id) + " added to vlan "
            + to_string(static_cast<uint32_t>(vlan_id)) + " as tagged.");
}

void NetworkController::m_add_untagged_port_to_vlan(uint16_t vlan_id, fm_int port_id) {
    uint32_t default_vlan = static_cast<uint32_t>(vlan_id);

    fm_status retval = fmAddVlanPort(m_switch_id, vlan_id, port_id, 0);
    m_log_error_and_throw(retval, "Cannot add port=" + to_string(port_id) +
                " to vlan=" + to_string(static_cast<uint32_t>(vlan_id)));

    retval = fmSetPortAttribute(m_switch_id, port_id, FM_PORT_DEF_VLAN, &default_vlan);
    m_log_error_and_throw(retval, "Cannot set default vlan "
                                     + to_string(static_cast<uint32_t>(default_vlan)) +
                                     + " to port=" + to_string(port_id));

    retval = fmSetVlanPortState(m_switch_id, vlan_id, port_id, FM_STP_STATE_FORWARDING);
    m_log_error_and_throw(retval, "Cannot set port=" + to_string(port_id) + " fowarding.");

    log_debug(GET_LOGGER("fm6000"), "Port " + to_string(port_id) + " added to vlan "
            + to_string(static_cast<uint32_t>(vlan_id)) + " as untagged.");
}

void NetworkController::m_log_error_and_throw(fm_status retval, const std::string& msg) {
    if (FM_OK != retval) {
        throw runtime_error(msg + " Status=" + to_string(retval));
    }
}

NetworkController::~NetworkController() {}
#endif
