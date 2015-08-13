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
 * @file switch_port_info.cpp
 *
 * @brief Switch port parameters
 * */

#include "switch_port_info.hpp"
#include "socket.hpp"
#include "message.hpp"
#include "vlan_message.hpp"
#include "ethtool.hpp"
#include "sysfs.hpp"
#include "network_config.hpp"

#include "hw/fm10000/network_controller_manager.hpp"
#include "hw/fm10000/network_controller.hpp"

#include <sstream>
#include <net/if.h>

using namespace agent::network::api::netlink;
using namespace agent::network::hw::fm10000;

SwitchPortInfo::SwitchPortInfo(const std::string& uuid,
                           const std::string& port) : SwitchPortInfo(port) {
    auto controller = NetworkControllerManager::get_network_controller();
    NetworkController::SwitchId switch_id = controller->get_switch_id(uuid);
    const auto& port_type = controller->get_switch_port_type(get_index());
    if (NetworkController::UNKNOWN_SWITCH_PORT_TYPE != port_type) {
        set_type(port_type);
    }
    set_switch_id(uint8_t(switch_id));
}

SwitchPortInfo::SwitchPortInfo(const std::string& port_identifier) {
    uint16_t sw_id{0};
    uint16_t sw_port{0};
    std::string sw_prefix{};
    char port_prefix{};
    std::istringstream stream(port_identifier);

    /* port identifier format: sw<Switch>p<Port> */
    stream.width(2);       /* set switch prefix length */
    stream >> sw_prefix;   /* read switch prefix */
    stream >> sw_id;       /* get switch index */
    stream >> port_prefix; /* read port prefix */
    stream >> sw_port;     /* get switch port index */
    if (!stream.eof() || stream.fail() ||
        "sw" != sw_prefix || 'p' != port_prefix) {
        throw std::runtime_error("wrong port identifier.");
    }
    set_is_present(0 != if_nametoindex(port_identifier.c_str()));
    set_switch_id(uint8_t(sw_id));
    set_index(uint8_t(sw_port));
}

SwitchPortInfo::~SwitchPortInfo() {}

void SwitchPortInfo::get_switch_port_link_state() {
    Socket socket;
    Message message;

    /* connect to the netlink */
    socket.connect();
    message.set_type(RTM_GETLINK);
    message.set_flags(NLM_F_REQUEST);
    message.set_switch(get_switch_id());
    message.set_port(get_index());
    socket.send(message);
    socket.receive(message);
    set_link_state(message.get_link_state());
    set_operational_state(message.get_operational_state());
}

void SwitchPortInfo::set_switch_port_link_state() {
    Socket socket;
    Message message;

    if (State::UNKNOWN == get_link_state_enum()) {
        throw std::runtime_error("unknown port state");
    }

    socket.connect();
    message.set_type(RTM_SETLINK);
    message.set_flags(NLM_F_REPLACE|NLM_F_REQUEST|NLM_F_ACK);
    message.set_switch(get_switch_id());
    message.set_port(get_index());
    message.set_link_state(get_link_state_enum());
    socket.send(message);
}

void SwitchPortInfo::set_switch_port_attr(SwAttr swattr, SwAttrValue value) {
    Socket socket;
    Message message;

    socket.connect();
    message.set_type(RTM_SETLINK);
    message.set_flags(NLM_F_REPLACE|NLM_F_REQUEST|NLM_F_ACK);
    message.set_switch(get_switch_id());
    message.set_port(get_index());
    message.set_switch_attr(swattr, value);
    socket.send_switch(message);
}

uint32_t SwitchPortInfo::get_switch_port_speed() {
    Ethtool ethtool(get_switch_id(), get_port_index());
    /* Ethtool operates in MBPS, return speed in GBPS */
    return ethtool.get_speed()/1000;
}

void SwitchPortInfo::set_switch_port_speed(uint32_t speed) {
    Ethtool ethtool(get_switch_id(), get_port_index());
    if (PortType::PCIE == get_type()) {
        /* skip setting speed for PCIe port */
        return;
    }
    /* need to convert from GBPS to MBPS */
    return ethtool.set_speed(speed * 1000);
}

void SwitchPortInfo::get_switch_port_attribute(PortAttribute attr,
                                               PortAttributeValue& value) {
    switch (attr) {
    case LINKSPEEDGBPS:
        value.set(get_switch_port_speed());
        break;
    case LINKSTATE:
        get_switch_port_link_state();
        value.set(get_link_state());
        break;
    case OPERATIONALSTATE:
        get_switch_port_link_state();
        value.set(get_operational_state());
        break;
    case PORTIDENTIFIER:
        value.set(Message::create_port_identifier(get_switch_id(),
                                                       get_index()));
        break;
    case FRAMESIZE:
        value.set(get_switch_port_max_frame_size());
        break;
    case AUTOSENSE:
        value.set(get_switch_port_autoneg());
        break;
    case VLANLIST:
        get_switch_vlan_list();
        break;
    case MACADDRESS:
        value.set(get_switch_port_mac_address());
        break;
    case STATUS:
    case PORTTYPE:
    case LINKTECHNOLOGY:
    case MAXSPEEDGBPS:
    case PORTWIDTH:
    case ISMANAGEMENTPORT:
    case LASTERRORCODE:
    case ERRORCLEARED:
    case LASTSTATECHANGETIME:
    case IPV4ADDRESS:
    case IPV6ADDRESS:
    case NEIGHBORINFO:
    case VLANENABLE:
        throw std::runtime_error("unsupported port attribute");
    default:
        throw std::runtime_error("unknown port attribute");
    }
}

void SwitchPortInfo::set_switch_port_attribute(PortAttribute attr,
                                               const PortAttributeValue& value
                                               ) {
    switch (attr) {
    case LINKSPEEDGBPS:
        set_switch_port_speed(value.get_number());
        break;
    case LINKSTATE:
        set_link_state(value.get_string());
        set_switch_port_link_state();
        break;
    case FRAMESIZE:
        set_switch_port_attr(IFLA_ATTR_MAX_FRAME_SIZE, value.get_number());
        break;
    case AUTOSENSE:
        set_switch_port_attr(IFLA_ATTR_AUTONEG, (value.get_bool() ? 1 : 0));
        break;
    case STATUS:
    case PORTTYPE:
    case LINKTECHNOLOGY:
    case MAXSPEEDGBPS:
    case PORTWIDTH:
    case ISMANAGEMENTPORT:
    case LASTERRORCODE:
    case ERRORCLEARED:
    case LASTSTATECHANGETIME:
    case MACADDRESS:
    case IPV4ADDRESS:
    case IPV6ADDRESS:
    case NEIGHBORINFO:
    case VLANENABLE:
    case VLANLIST:
        throw std::runtime_error("unsupported port attribute");
    case OPERATIONALSTATE:
    case PORTIDENTIFIER:
        throw std::runtime_error("read-only port attribute");
    default:
        throw std::runtime_error("unknown port attribute");
    }
}

uint32_t SwitchPortInfo::get_switch_port_max_frame_size() {
    auto ifname = Message::create_port_identifier(get_switch_id(),
                                                  get_index());
    SysFs sysfs(ifname);
    return sysfs.get_max_frame_size();
}

bool SwitchPortInfo::get_switch_port_autoneg() {
    auto ifname = Message::create_port_identifier(get_switch_id(),
                                                  get_index());
    SysFs sysfs(ifname);
    if (PortType::PCIE == get_type()) {
        /* return noautoneg(false) for PCIe port */
        return false;
    }
    return (0 == sysfs.get_autoneg() ? false : true);
}

void SwitchPortInfo::get_switch_vlan_list() {
    VlanList vlist{};
    Socket socket;
    socket.connect();
    InfoVlanPortMessage vlan_msg;
    socket.send_message(vlan_msg);
    socket.receive_message(vlan_msg);
    for (const auto& vlan_info : vlan_msg.get_vlan_list(get_index())) {
        vlist.push_back(std::to_string(int(vlan_info.m_vlan_id)));
    }
    set_vlan_list(vlist);
}

string SwitchPortInfo::get_switch_port_mac_address() {
    Socket socket;
    Message message;

    if (PortType::PCIE != get_type()) {
        /* we can get host MAC address only for PCIe ports */
        return "";
    }

    /* connect to the netlink */
    socket.connect();
    message.set_type(RTM_GETNEIGH);
    message.set_flags(NLM_F_DUMP | NLM_F_REQUEST);
    message.set_switch(get_switch_id());
    message.set_port(get_index());
    socket.modify_callback(Message::read_neighbour, &message);
    socket.send(message);
    socket.receive_to_callback(message);
    return message.get_address();
}
