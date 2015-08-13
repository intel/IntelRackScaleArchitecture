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
 * @brief IES Vlan functionality.
 * */

#include "switch_vlan.hpp"
#include "logger/logger_factory.hpp"
#include "hw/fm6000/network_controller_manager.hpp"
#include "hw/fm6000/network_controller.hpp"

#ifdef IES_FOUND
using namespace agent::network::api::ies;
using namespace agent::network::hw::fm6000;

static constexpr int SWITCH_VLAN_ID_1 = 1;
static constexpr int SWITCH_VLAN_NOT_FOUND_VALUE = -1;
static constexpr int SWITCH_VLAN_PORT_NOT_FOUND_VALUE = -1;

SwitchVlan::SwitchVlan(const std::string& switch_uuid,
                       const std::string& vlan) {
    fm_int vlan_id{0};
    auto controller = NetworkControllerManager::get_network_controller();
    NetworkController::SwitchId switch_id = controller->get_switch_id(switch_uuid);
    set_switch_id(SwitchVlan::SwitchId(switch_id));
    try{
        vlan_id = static_cast<fm_int>(std::stoi(vlan));
    }
    catch (const std::invalid_argument&) {
        log_error(GET_LOGGER("fm6000"), "Cannot convert port identifier to port id");
        throw std::runtime_error("Cannot convert port identifier to port id");
    }
    set_is_present(get_switch_vlan_is_present(get_switch_id(), vlan_id));
    set_vlan_id(SwitchVlan::VlanId(vlan_id));
}

bool SwitchVlan::get_switch_vlan_is_present(fm_int switch_id, fm_int vlan_id) {
    fm_status retval{FM_FAIL};
    fm_int vlan_id_value{SWITCH_VLAN_NOT_FOUND_VALUE};
    bool is_present{false};

    retval = fmGetVlanFirst(switch_id, &vlan_id_value);
    while (FM_OK == retval && SWITCH_VLAN_NOT_FOUND_VALUE != vlan_id_value) {
        if (vlan_id == vlan_id_value) {
            is_present = true;
            break;
        }
        retval = fmGetVlanNext(switch_id, vlan_id_value, &vlan_id_value);
    }
    log_debug(GET_LOGGER("fm6000"), "CHECK VLAN EXISTENCE:"
                  << " vlan=" << vlan_id
                  << " is present=" << (is_present ? "true" : "false") << "\n");
    return is_present;
}

bool SwitchVlan::is_port_present(const std::string& port) {
    fm_int port_id{0};
    try{
        port_id = static_cast<fm_int>(std::stoi(port));
    }
    catch (const std::invalid_argument&) {
        log_error(GET_LOGGER("fm6000"), "Cannot convert port identifier to port id");
        throw std::runtime_error("Cannot convert port identifier to port id");
    }
    return get_switch_vlan_port_is_present(get_switch_id(), get_vlan_id(), port_id);
}

bool SwitchVlan::get_switch_vlan_port_is_present(fm_int switch_id, fm_int vlan_id, fm_int port_id) {
    fm_status retval{FM_FAIL};
    fm_int port_id_value{SWITCH_VLAN_NOT_FOUND_VALUE};
    bool is_present{false};

    retval = fmGetVlanPortFirst(switch_id, vlan_id, &port_id_value);
    while (FM_OK == retval && SWITCH_VLAN_PORT_NOT_FOUND_VALUE != port_id_value) {
        if (port_id == port_id_value) {
            is_present = true;
            break;
        }
        retval = fmGetVlanPortNext(switch_id, vlan_id, port_id_value, &port_id_value);
    }
    log_debug(GET_LOGGER("fm6000"), "CHECK VLAN PORT EXISTENCE:"
                  << " vlan=" << vlan_id
                  << " port=" << port_id
                  << " is present=" << (is_present ? "true" : "false") << "\n");
    return is_present;
}

SwitchVlan::VlanList SwitchVlan::read_switch_port_vlan_list(
    SwitchId switch_id, PortId port_id) {
    fm_status retval{FM_FAIL};
    fm_int vlan_id{SWITCH_VLAN_NOT_FOUND_VALUE};
    VlanList vlist{};

    retval = fmGetVlanFirst(switch_id, &vlan_id);
    while (FM_OK == retval && SWITCH_VLAN_NOT_FOUND_VALUE != vlan_id) {
        if (get_switch_vlan_port_is_present(switch_id, vlan_id, port_id)) {
            vlist.push_back(std::to_string(vlan_id));
        }
        retval = fmGetVlanNext(switch_id, vlan_id, &vlan_id);
    }

    return vlist;
}

void SwitchVlan::write_switch_vlan_attribute(
    SwitchId switch_id, VlanId vlan_id, fm_int attr, void* attr_value_ptr) {
    fm_status retval{FM_FAIL};

    if (nullptr == attr_value_ptr) {
        throw runtime_error("Cannot execute fmSetVlanAttributte: null pointer input parameter");
    }

    retval = fmSetVlanAttribute(switch_id, vlan_id, attr, attr_value_ptr);
    if (FM_OK != retval) {
        throw runtime_error("Cannot execute fmSetVlanAttribute: switch=" + to_string(static_cast<int>(switch_id)) +
                            " vlan=" + to_string(static_cast<int>(vlan_id)) +
                            " attr=" + to_string(attr) +
                            " retval=" + to_string(retval));
    }
    log_debug(GET_LOGGER("fm6000"), "Set vlan attribute:"
              << " vlan=" << vlan_id
              << " attr=" << attr << "\n");
}

void SwitchVlan::create_switch_vlan(
    SwitchId switch_id, VlanId vlan_id, bool* vlan_created_ptr) {
    fm_status retval{FM_FAIL};

    if (nullptr == vlan_created_ptr) {
        throw runtime_error("Cannot execute fmCreateVlan: null pointer input parameter");
    }

    *vlan_created_ptr = false;

    retval = fmCreateVlan(switch_id, vlan_id);
    if ((FM_OK != retval) && (FM_ERR_VLAN_ALREADY_EXISTS != retval)) {
        throw runtime_error("Cannot execute fmCreateVlan: switch=" + to_string(static_cast<int>(switch_id)) +
                            " vlan=" + to_string(static_cast<int>(vlan_id)) +
                            " retval=" + to_string(retval));
    }

    if (FM_OK == retval) {
        log_debug(GET_LOGGER("fm6000"), "CREATED VLAN:"
                  << " vlan=" << vlan_id
                  << " retval=" << retval << "\n");
        *vlan_created_ptr = true;
    }

    if (FM_ERR_VLAN_ALREADY_EXISTS == retval) {
        log_debug(GET_LOGGER("fm6000"), "VLAN ALREADY EXIST:"
                  << " vlan=" << vlan_id
                  << " retval=" << retval << "\n");
    }
}

void SwitchVlan::read_switch_vlan_port_tag(
    fm_int switch_id, fm_int vlan_id, fm_int port_id, fm_bool* tag_value_ptr) {
    fm_status retval{FM_FAIL};

    if (nullptr == tag_value_ptr) {
        throw runtime_error("Cannot execute fmGetVlanPortTag: null pointer input parameter");
    }

    retval = fmGetVlanPortTag(switch_id, vlan_id, port_id, tag_value_ptr);
    if (FM_OK != retval) {
        throw runtime_error("Cannot read vlan port tag parameter: switch=" + to_string(int(switch_id)) +
                            " vlan=" + to_string(int(vlan_id)) +
                            " port=" + to_string(int(port_id)));
    }
    log_debug(GET_LOGGER("fm6000"), "Read vlan port tag parameter:"
              << " vlan=" << vlan_id
              << " port=" << port_id
              << " tag_value=" << (*tag_value_ptr ? "true" : "false") << "\n");
}

fm_status SwitchVlan::read_vlan_port_params(
    SwitchId switch_id, VlanId vlan_id, PortId port_id,
    bool *is_member_ptr, bool *is_pvid_ptr, bool *is_tagged_ptr) {
    fm_status retval{FM_FAIL};
    fm_int attr{0};
    fm_int vid_value{-1};
    fm_bool tag_value;

    if (nullptr == is_member_ptr || nullptr == is_pvid_ptr || nullptr == is_tagged_ptr) {
        log_debug(GET_LOGGER("fm6000"), "Cannot read vlan port params: null pointer input parameter");
        return FM_FAIL;
    }

    *is_member_ptr = false;
    *is_pvid_ptr = false;
    *is_tagged_ptr = false;

    retval = fmGetVlanPortTag(switch_id, vlan_id, port_id, &tag_value);
    if (FM_ERR_INVALID_PORT == retval) {
        return FM_OK;
    }
    if (FM_OK != retval) {
        log_debug(GET_LOGGER("fm6000"), "Cannot execute fmGetVlanPortTag:"
                  << " vlan=" << vlan_id
                  << " port=" << port_id
                  << " retval=" << retval << "\n");
        return retval;
    }

    *is_member_ptr = true;
    *is_tagged_ptr = tag_value;

    attr = FM_PORT_DEF_VLAN;
    retval = fmGetPortAttribute(switch_id, port_id, attr, &vid_value);
    if (FM_OK != retval) {
        log_debug(GET_LOGGER("fm6000"), "Cannot execute fmGetPortAttribute:"
                  << " port=" << port_id
                  << " attr=" << int(attr)
                  << " attr_value=" << vid_value
                  << " retval=" << retval << "\n");
        return retval;
    }

    if (vlan_id == vid_value) {
        fm_bool drop_untagged_value = FM_DISABLED;
        attr = FM_PORT_DROP_UNTAGGED;
        if (SWITCH_VLAN_ID_1 == vlan_id) {
            // VID=1 is a special case. IES API does not allow for no default VLAN on a port.
            // Therefore, when the PVID vlan is removed from the port, the port's default
            // VLAN is set to 1 and the port is configured to drop untagged packets on ingress.
            // We need to check that the port does not drop untagged packets before we
            // declare VID=1 as its PVID
            retval = fmGetPortAttribute(switch_id, port_id,
                                        attr, &drop_untagged_value);
            if (FM_OK != retval ) {
                log_debug(GET_LOGGER("fm6000"), "Cannot execute fmGetPortAttribute:"
                          << " port=" << port_id
                          << " attr=" << int(attr)
                          << " attr_value=" << drop_untagged_value
                          << " retval=" << retval << "\n");
                return retval;
            }
        }
        *is_pvid_ptr = (FM_DISABLED == drop_untagged_value);
    }

    return FM_OK;
}

void SwitchVlan::add_port_cleanup_and_throw(
    SwitchId switch_id, VlanId vlan_id, PortId port_id, bool vlan_port_added, bool vlan_created) {
    fm_status retval{FM_FAIL};

    if (vlan_port_added) {
        retval = fmDeleteVlanPort(switch_id, vlan_id, port_id);
        if (FM_OK != retval) {
            log_debug(GET_LOGGER("fm6000"), "Cannot execute fmDeleteVlanPort:"
                      << " vlan=" << vlan_id
                      << " port=" << port_id
                      << " retval=" << retval << "\n");
        }
    }

    if (vlan_created) {
        retval = fmDeleteVlan(switch_id, vlan_id);
        if (FM_OK != retval) {
            log_debug(GET_LOGGER("fm6000"), "Cannot execute fmDeleteVlan:"
                      << " vlan=" << vlan_id
                      << " retval=" << retval << "\n");
        }
    }

    throw runtime_error("Cannot add port to vlan: switch=" + to_string(int(switch_id)) +
                        " vlan=" + to_string(int(vlan_id)) +
                        " port=" + to_string(int(port_id)));
}

void SwitchVlan::add_port(const PortIdentifier& port, bool tagged) {
    fm_status retval{FM_FAIL};
    bool vlan_created{false};
    bool vlan_port_added{false};
    bool curr_member{false};
    bool curr_pvid{false};
    bool curr_tagged{false};
    fm_int mtu_value{0};

    PortId port_id = PortId(std::stoi(port));

    create_switch_vlan(get_switch_id(), get_vlan_id(), &vlan_created);

    if (true == vlan_created) {
        write_switch_vlan_attribute(get_switch_id(), get_vlan_id(),
                                    FM_VLAN_MTU, static_cast<void*>(&mtu_value));
    }
    else {
        retval = read_vlan_port_params(get_switch_id(), get_vlan_id(), port_id,
                                       &curr_member, &curr_pvid, &curr_tagged);
        if (FM_OK != retval || curr_member) {
            add_port_cleanup_and_throw(get_switch_id(), get_vlan_id(), port_id, vlan_port_added, vlan_created);
        }

        log_debug(GET_LOGGER("fm6000"), "Read vlan port params:"
                  << " vlan=" << get_vlan_id()
                  << " port=" << port_id
                  << " pvid=" << (curr_pvid ? "true" : "false")
                  << " tagged=" << (curr_tagged ? "true" : "false") << "\n");
    }

    if (!curr_member) {
        fm_bool tag_value = tagged;
        retval = fmAddVlanPort(get_switch_id(), get_vlan_id(), port_id, tag_value);
        if (FM_OK != retval) {
            log_debug(GET_LOGGER("fm6000"), "Cannot execute fmAddVlanPort:"
                      << " vlan=" << get_vlan_id()
                      << " port=" << port_id
                      << " tag="  << tag_value
                      << " retval=" << retval << "\n");
            add_port_cleanup_and_throw(get_switch_id(), get_vlan_id(), port_id, vlan_port_added, vlan_created);
        }
        vlan_port_added = true;
        log_debug(GET_LOGGER("fm6000"), "ADDED VLAN PORT:"
                  << " vlan=" << get_vlan_id()
                  << " port=" << port_id
                  << " tag=" << (tag_value ? "true" : "false") << "\n");
    }


    // @TODO add make_pvid flag to make decision weather pvid should be set or not
    if (!curr_pvid) {
        fm_int attr{0};
        fm_uint32 def_vlan_value = get_vlan_id();
        fm_bool drop_untagged_value = FM_DISABLED;

        attr = FM_PORT_DEF_VLAN;
        retval = fmSetPortAttribute(get_switch_id(), port_id, attr, &def_vlan_value);
        if (FM_OK != retval) {
            log_debug(GET_LOGGER("fm6000"), "Cannot execute fmSetPortAttributte:"
                      << " vlan=" << get_vlan_id()
                      << " port=" << port_id
                      << " attr=" << attr
                      << " attr_value="  << def_vlan_value
                      << " retval=" << retval << "\n");
            add_port_cleanup_and_throw(get_switch_id(), get_vlan_id(), port_id, vlan_port_added, vlan_created);
        }

        // When a PVID is set for a port it should allow untagged packets on ingress.
        // This reverses the setting done when a port is deleted from its PVID.

        // @TODO check if port != SW_PORT_TYPE_CPU
        // This attribute is read-only on the CPU port
        attr = FM_PORT_DROP_UNTAGGED;
        retval = fmSetPortAttribute(get_switch_id(), port_id, attr, &drop_untagged_value);
        if (FM_OK != retval) {
            log_debug(GET_LOGGER("fm6000"), "Cannot execute fmSetPortAttributte:"
                      << " vlan=" << get_vlan_id()
                      << " port=" << port_id
                      << " attr=" << attr
                      << " attr_value="  << drop_untagged_value
                      << " retval=" << retval << "\n");
            add_port_cleanup_and_throw(get_switch_id(), get_vlan_id(), port_id, vlan_port_added, vlan_created);
        }
    }
}

void SwitchVlan::remove_port(const PortIdentifier& port) {
    fm_status retval{FM_FAIL};
    fm_int first_port;
    fm_int attr{0};
    fm_uint32 vid_value{0};

    PortId port_id = PortId(std::stoi(port));

    if (!get_switch_vlan_port_is_present(get_switch_id(), get_vlan_id(), port_id)) {
        throw runtime_error("Cannot find port on vlan: switch=" + to_string(int(get_switch_id())) +
                            " vlan=" + to_string(int(get_vlan_id())) +
                            " port=" + to_string(int(port_id)));
    }

    attr = FM_PORT_DEF_VLAN;
    retval = fmGetPortAttribute(get_switch_id(), port_id, attr, &vid_value);
    if (FM_OK != retval) {
        log_debug(GET_LOGGER("fm6000"), "Cannot execute fmGetPortAttribute:"
                  << " port=" << port_id
                  << " attr=" << int(attr)
                  << " attr_value=" << vid_value
                  << " retval=" << retval << "\n");

    }
    else if (get_vlan_id() == vid_value) {
        // The port PVID is being deleted from the port. We need to set it back to VID 1 in the HW.
        vid_value = SWITCH_VLAN_ID_1;
        retval = fmSetPortAttribute(get_switch_id(), port_id, attr, &vid_value);
        if (FM_OK != retval) {
            log_debug(GET_LOGGER("fm6000"), "Cannot execute fmSetPortAttribute:"
                      << " port=" << port_id
                      << " attr=" << int(attr)
                      << " attr_value=" << vid_value
                      << " retval=" << retval << "\n");
        }

        // This attribute is read-only on the CPU port
        fm_bool drop_untagged_value = FM_ENABLED;
        attr = FM_PORT_DROP_UNTAGGED;
        retval = fmSetPortAttribute(get_switch_id(), port_id, attr, &drop_untagged_value);
        if (FM_OK != retval) {
            log_debug(GET_LOGGER("fm6000"), "Cannot execute fmSetPortAttributte:"
                      << " vlan=" << get_vlan_id()
                      << " port=" << port_id
                      << " attr=" << attr
                      << " attr_value="  << drop_untagged_value
                      << " retval=" << retval << "\n");
        }
    }

    retval = fmDeleteVlanPort(get_switch_id(), get_vlan_id(), port_id);
    if (FM_OK != retval) {
        log_debug(GET_LOGGER("fm6000"), "Cannot execute fmDeleteVlanPort:"
                  << " vlan=" << get_vlan_id()
                  << " port=" << port_id
                  << " retval=" << retval << "\n");
        throw runtime_error("Cannot delete port from vlan: switch=" + to_string(int(get_switch_id())) +
                            " vlan=" + to_string(int(get_vlan_id())) +
                            " port=" + to_string(int(port_id)));
    }
    log_debug(GET_LOGGER("fm6000"), "DELETE VLAN PORT:"
              << " vlan=" << get_vlan_id()
              << " port=" << port_id << "\n");

    retval = fmGetVlanPortFirst(get_switch_id(), get_vlan_id(), &first_port);
    if (FM_OK != retval) {
        log_debug(GET_LOGGER("fm6000"), "Cannot execute fmGetVlanPortFirst:"
                  << " vlan=" << get_vlan_id()
                  << " retval=" << retval << "\n");

    }
    else if (SWITCH_VLAN_PORT_NOT_FOUND_VALUE == first_port) {
        // There are no more ports in the VLAN - delete the VLAN
        retval = fmDeleteVlan(get_switch_id(), get_vlan_id());
        if (FM_OK != retval) {
            log_debug(GET_LOGGER("fm6000"), "Cannot execute fmDeleteVlan:"
                      << " vlan=" << get_vlan_id()
                      << " retval=" << retval << "\n");
        }
	else {
            log_debug(GET_LOGGER("fm6000"), "DELETE VLAN:"
                      << " vlan=" << get_vlan_id() << "\n");
        }
    }
}

void SwitchVlan::get_info(const PortIdentifier& port, VlanPortInfo& info) {
    fm_bool tag_value;
    fm_int port_id{0};

    try{
        port_id = static_cast<fm_int>(std::stoi(port));
    }
    catch (runtime_error& error) {
        log_error(GET_LOGGER("fm6000"), "Cannot convert port identifier to port id");
        log_debug(GET_LOGGER("fm6000"), error.what());
        throw std::runtime_error("Cannot convert port identifier to port id");
    }

    read_switch_vlan_port_tag(get_switch_id(), get_vlan_id(), port_id, &tag_value);
    info.set_tagged(tag_value);
}
#endif /* IES_FOUND */
