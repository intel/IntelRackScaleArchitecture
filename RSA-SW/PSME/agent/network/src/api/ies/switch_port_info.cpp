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
 * @brief ...
 * */

#include "switch_port_info.hpp"
#include "switch_vlan.hpp"
#include "api/switch_vlan.hpp"
#include "logger/logger_factory.hpp"
#include "hw/fm6000/network_controller.hpp"
#include "hw/fm6000/network_controller_manager.hpp"

#ifdef IES_FOUND
using namespace agent::network::api::ies;
using namespace agent::network::hw::fm6000;

SwitchPortInfo::SwitchPortInfo(const std::string& uuid,
                               const std::string& port_identifier) {
    try {
        set_index(static_cast<uint8_t>(std::stoi(port_identifier)));
    }
    catch (const std::invalid_argument&) {
        throw std::runtime_error("Cannot convert port identifier to port id");
    }
    auto controller = NetworkControllerManager::get_network_controller();
    NetworkController::SwitchId switch_id = controller->get_switch_id(uuid);
    set_is_present(get_switch_port_is_present(get_index()));
    set_switch_id(uint8_t(switch_id));
}

SwitchPortInfo::~SwitchPortInfo() {}

const SwitchPortInfo::iesmode2state_t SwitchPortInfo::m_iesmode2state = {{
    std::make_pair(FM_PORT_MODE_UP,            State::UP),
    std::make_pair(FM_PORT_MODE_ADMIN_PWRDOWN, State::DOWN),
    std::make_pair(FM_PORT_MODE_ADMIN_DOWN,    State::DOWN),
}};

const SwitchPortInfo::iesstate2state_t SwitchPortInfo::m_iesstate2state = {{
    std::make_pair(FM_PORT_STATE_UP,            State::UP),
    std::make_pair(FM_PORT_STATE_ADMIN_PWRDOWN, State::DOWN),
    std::make_pair(FM_PORT_STATE_ADMIN_DOWN,    State::DOWN),
    std::make_pair(FM_PORT_STATE_DOWN,          State::DOWN),
}};

const SwitchPortInfo::iesattr2attr_t SwitchPortInfo::m_iesattr2attr = {{
    std::make_pair(FM_PORT_SPEED,          PortAttribute::LINKSPEEDGBPS),
    std::make_pair(FM_PORT_MAX_FRAME_SIZE, PortAttribute::FRAMESIZE),
}};

const SwitchPortInfo::iescap2speed_t SwitchPortInfo::m_iescap2speed = {{
    std::make_pair(FM_PORT_CAPABILITY_SPEED_10M,   SpeedCapability::LINKSPEED10M),
    std::make_pair(FM_PORT_CAPABILITY_SPEED_100M,  SpeedCapability::LINKSPEED100M),
    std::make_pair(FM_PORT_CAPABILITY_SPEED_1G,    SpeedCapability::LINKSPEED1G),
    std::make_pair(FM_PORT_CAPABILITY_SPEED_2PT5G, SpeedCapability::LINKSPEED2PT5G),
    std::make_pair(FM_PORT_CAPABILITY_SPEED_5G,    SpeedCapability::LINKSPEED5G),
    std::make_pair(FM_PORT_CAPABILITY_SPEED_10G,   SpeedCapability::LINKSPEED10G),
    std::make_pair(FM_PORT_CAPABILITY_SPEED_20G,   SpeedCapability::LINKSPEED20G),
    std::make_pair(FM_PORT_CAPABILITY_SPEED_40G,   SpeedCapability::LINKSPEED40G),
}};

static constexpr int SWITCH_PORT_SPEED_DISABLE = 0;
static constexpr int SWITCH_PORT_SPEED_SGMII   = 1;
static constexpr int SWITCH_PORT_SPEED_10G     = 10;

static constexpr int SWITCH_PORT_MAX_FRAME_SIZE = 15864;

fm_int SwitchPortInfo::mode_to_ies(SwitchPortInfo::State state) const {
    for (const auto& elem : m_iesmode2state) {
        if (elem.second == state) {
            return elem.first;
        }
    }
    throw std::runtime_error("cannot convert enum state to iesmode");
}

SwitchPortInfo::State SwitchPortInfo::mode_to_enum(const fm_int iesmode) const {
    for (const auto& elem : m_iesmode2state) {
        if (elem.first == iesmode) {
            return elem.second;
        }
    }
    return State::UNKNOWN;
}

fm_int SwitchPortInfo::state_to_ies(SwitchPortInfo::State state) const {
    for (const auto& elem : m_iesstate2state) {
        if (elem.second == state) {
            return elem.first;
        }
    }
    throw std::runtime_error("cannot convert enum state to iesstate");
}

SwitchPortInfo::State SwitchPortInfo::state_to_enum(const fm_int iesstate) const {
    for (const auto& elem : m_iesstate2state) {
        if (elem.first == iesstate) {
            return elem.second;
        }
    }
    return State::UNKNOWN;
}

fm_int SwitchPortInfo::attr_to_ies(SwitchPortInfo::PortAttribute attr) const {
    for (const auto& elem : m_iesattr2attr) {
        if (elem.second == attr) {
            return elem.first;
        }
    }
    throw std::runtime_error("cannot convert enum attr to iesattr");
}

SwitchPortInfo::PortAttribute SwitchPortInfo::attr_to_enum(const fm_int iesattr) const {
    for (const auto& elem : m_iesattr2attr) {
        if (elem.first == iesattr) {
            return elem.second;
        }
    }
    throw std::runtime_error("cannot convert enum iesattr to attr");
}

fm_uint32 SwitchPortInfo::speed_to_iescap(fm_uint32 speed) const {
    for (const auto& elem : m_iescap2speed) {
        if (elem.second == speed) {
            return elem.first;
        }
    }
    throw std::runtime_error("Speed value not supported");
}

bool SwitchPortInfo::get_switch_port_is_present(fm_int port_index) {
    auto network_controller = NetworkControllerManager::get_network_controller();
    uint16_t port_count = uint16_t(network_controller->get_ports_count());
    return (0 < port_index && port_count >= port_index);
}

void SwitchPortInfo::read_switch_port_state(
    fm_int switch_id, fm_int port_index, fm_int* mode_ptr, fm_int* state_ptr, fm_int* info_ptr) {
    fm_status retval = FM_FAIL;

    if (nullptr == mode_ptr || nullptr == state_ptr || nullptr == info_ptr) {
        throw runtime_error("Cannot execute fmGetPortStateV2: null pointer input parameter");
    }

    retval = fmGetPortStateV2(switch_id, port_index, FM_PORT_ACTIVE_MAC, mode_ptr, state_ptr, info_ptr);
    if (FM_OK != retval) {
        throw runtime_error("Cannot execute fmGetPortStateV2: switch=" + to_string(switch_id) +
                          " port=" + to_string(port_index) +
                          " retval=" + to_string(retval));
    }
    log_debug(GET_LOGGER("fm6000"), "Get port state: switch=" << switch_id
              << " port="  << port_index
              << " mode="  << *mode_ptr
              << " state=" << *state_ptr << "\n");
}

void SwitchPortInfo::write_switch_port_state(
    fm_int switch_id, fm_int port_index, fm_int mode) {
    fm_status retval = FM_FAIL;

    retval = fmSetPortStateV2(switch_id, port_index, FM_PORT_ACTIVE_MAC, mode, 0);
    if (FM_OK != retval) {
        throw runtime_error("Cannot execute fmSetPortStateV2: switch=" + to_string(switch_id) +
                            " port=" + to_string(port_index) +
                            " mode=" + to_string(mode) +
                            " retval=" + to_string(retval));
    }
    log_debug(GET_LOGGER("fm6000"), "Set port state: switch=" << switch_id
              << " port=" << port_index
              << " mode=" << mode << "\n");
}

void SwitchPortInfo::write_switch_port_speed(
    fm_int switch_id, fm_int port_index, fm_uint32 speed) {
    fm_uint64 eth_mode_value;

    switch (speed) {
    case SWITCH_PORT_SPEED_DISABLE:
        eth_mode_value = FM_ETH_MODE_DISABLED;
        break;
    case SWITCH_PORT_SPEED_SGMII:
        eth_mode_value = FM_ETH_MODE_SGMII;
	break;
    case SWITCH_PORT_SPEED_10G:
        eth_mode_value = FM_ETH_MODE_10GBASE_CR;
        break;
    default:
        throw std::runtime_error("Speed can not be set on port=" + to_string(port_index));
    }

    write_switch_port_attribute(switch_id, port_index,
        FM_PORT_ACTIVE_MAC, FM_PORT_LANE_ALL, FM_PORT_ETHERNET_INTERFACE_MODE, &eth_mode_value);
}

void SwitchPortInfo::write_switch_port_max_frame_size(
    fm_int switch_id, fm_int port_index, fm_int max_frame_size) {
    fm_uint64 value = fm_uint64(max_frame_size);

    if (0 <= max_frame_size && SWITCH_PORT_MAX_FRAME_SIZE >= max_frame_size) {
        write_switch_port_attribute(switch_id, port_index,
                FM_PORT_ACTIVE_MAC, FM_PORT_LANE_ALL, FM_PORT_MAX_FRAME_SIZE, &value);
    }
    else {
        throw runtime_error("Cannot set max frame size (out or range): switch=" + to_string(switch_id) +
                            " port=" + to_string(port_index) +
                            " max frame size=" + to_string(max_frame_size) +
                            " acceptable range=0.." + to_string(SWITCH_PORT_MAX_FRAME_SIZE));
    }
    log_debug(GET_LOGGER("fm6000"), "Set port max frame size: switch=" << switch_id
              << " port=" << port_index
              << " max frame size=" << max_frame_size << "\n");
}

void SwitchPortInfo::read_switch_port_capabilities(
    fm_int switch_id, fm_int port_index, fm_uint32* cap_value_ptr) {
    fm_status retval = FM_FAIL;

    if (nullptr == cap_value_ptr) {
        throw runtime_error("Cannot execute fmPlatformGetPortCapabilities: null pointer input parameter");
    }

    retval = fmPlatformGetPortCapabilities(switch_id, port_index, cap_value_ptr);
    if (FM_OK != retval) {
        throw runtime_error("Cannot execute fmPlatformGetPortCapabilities: switch=" + to_string(switch_id) +
                          " port=" + to_string(port_index) +
                          " retval=" + to_string(retval));
    }
    log_debug(GET_LOGGER("fm6000"), "Get port capabilities: switch=" << switch_id
              << " port=" << port_index
              << " cap_value=" << *cap_value_ptr << "\n");
}

void SwitchPortInfo::read_switch_port_attribute(
    fm_int switch_id, fm_int port_index, fm_int attr, fm_uint64* attr_value_ptr) {
    fm_status retval = FM_FAIL;

    if (nullptr == attr_value_ptr) {
        throw runtime_error("Cannot execute fmGetPortAttribute: null pointer input parameter");
    }

    retval = fmGetPortAttribute(switch_id, port_index, attr, attr_value_ptr);
    if (FM_OK != retval) {
        throw runtime_error("Cannot execute fmGetPortAttribute: switch=" + to_string(switch_id) +
                          " port=" + to_string(port_index) +
                          " attr=" + to_string(attr) +
                          " retval=" + to_string(retval));
    }
    log_debug(GET_LOGGER("fm6000"), "Get port attribute: switch=" << switch_id
              << " port=" << port_index
              << " attr=" << attr
              << " attr_value=" << *attr_value_ptr << "\n");
}

void SwitchPortInfo::write_switch_port_attribute(
    fm_int switch_id, fm_int port_index, fm_int mac, fm_int line, fm_int attr, fm_uint64* attr_value_ptr) {
    fm_status retval = FM_FAIL;

    if (nullptr == attr_value_ptr) {
        throw runtime_error("Cannot execute fmSetPortAttributteV2: null pointer input parameter");
    }

    retval = fmSetPortAttributeV2(switch_id, port_index, mac, line, attr, attr_value_ptr);
    if (FM_OK != retval) {
      throw runtime_error("Cannot execute fmSetPortAttributeV2: switch=" + to_string(switch_id) +
                          " port=" + to_string(port_index) +
                          " mac="  + to_string(mac) +
                          " line=" + to_string(line) +
                          " attr=" + to_string(attr) +
                          " attr_value=" + to_string(*attr_value_ptr) +
                          " retval=" + to_string(retval));
    }
    log_debug(GET_LOGGER("fm6000"), "Set port attribute: switch=" << switch_id
              << " port=" << port_index
              << " mac="  << mac
              << " line=" << line
              << " attr=" << attr
              << " attr_value=" << *attr_value_ptr << "\n");
}

void SwitchPortInfo::check_switch_port_speed_configurability() {
    fm_uint64 attr_value = 0;

    read_switch_port_attribute(get_switch_id(), get_index(),
                               FM_PORT_ETHERNET_INTERFACE_MODE, &attr_value);
    if (FM_ETH_MODE_SGMII != attr_value) {
        throw std::runtime_error("Speed can not be set on port=" + to_string(static_cast<int>(get_index())) +
                                 " for Ethernet-mode=" + to_string(attr_value) + " (not equal SGMII)");
    }
}

void SwitchPortInfo::check_switch_port_speed_capabilities(fm_uint32 speed) {
    fm_uint32 cap_value = 0;

    read_switch_port_capabilities(get_switch_id(), get_index(), &cap_value);
    if (!(speed_to_iescap(speed) & cap_value)) {
        throw std::runtime_error("Speed=" + to_string(speed) +
                                 " can not be set on port=" + to_string(static_cast<int>(get_index())) +
                                 " beacuse of capabilities=" + to_string(cap_value));
    }
}

void SwitchPortInfo::get_switch_port_attribute(PortAttribute attr) {
    switch (attr) {
    case LINKSPEEDGBPS:
        read_switch_port_attribute(get_switch_id(), get_index(),
                                   attr_to_ies(attr), &m_attr_value);
        set_link_speed(static_cast<uint32_t>(m_attr_value));
        break;
    case LINKSTATE:
        read_switch_port_state(get_switch_id(), get_index(),
                               &m_mode, &m_state, &m_info);
        set_link_state(mode_to_enum(m_mode));
        break;
    case OPERATIONALSTATE:
        read_switch_port_state(get_switch_id(), get_index(),
                               &m_mode, &m_state, &m_info);
        set_operational_state(state_to_enum(m_state));
        break;
    case PORTIDENTIFIER:
        set_identifier(std::to_string(static_cast<int>(get_index())));
        break;
    case FRAMESIZE:
        read_switch_port_attribute(get_switch_id(), get_index(),
                                   attr_to_ies(attr), &m_attr_value);
        set_max_frame_size(static_cast<uint16_t>(m_attr_value));
        break;
    case VLANLIST:
        set_vlan_list(SwitchVlan::read_switch_port_vlan_list(get_switch_id(), get_index()));
        break;
    case STATUS:
    case PORTTYPE:
    case LINKTECHNOLOGY:
    case MAXSPEEDGBPS:
    case PORTWIDTH:
    case AUTOSENSE:
    case ISMANAGEMENTPORT:
    case LASTERRORCODE:
    case ERRORCLEARED:
    case LASTSTATECHANGETIME:
    case MACADDRESS:
    case IPV4ADDRESS:
    case IPV6ADDRESS:
    case NEIGHBORINFO:
    case VLANENABLE:
    default:
        throw std::runtime_error("unknown port attribute");
    }
}

void SwitchPortInfo::set_switch_port_attribute(PortAttribute attr) {
    switch (attr) {
    case LINKSPEEDGBPS:
        write_switch_port_speed(get_switch_id(), get_index(), get_link_speed());
        break;
    case LINKSTATE:
        write_switch_port_state(get_switch_id(), get_index(),
                                mode_to_ies(get_link_state_enum()));
        break;
    case FRAMESIZE:
        write_switch_port_max_frame_size(get_switch_id(), get_index(),
                                         static_cast<fm_int>(get_max_frame_size()));
      break;
    case AUTOSENSE:
    case STATUS:
    case PORTIDENTIFIER:
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
        throw std::runtime_error("read-only port attribute");
    default:
        throw std::runtime_error("unknown port attribute");
    }
}
#endif /* IES_FOUND */
