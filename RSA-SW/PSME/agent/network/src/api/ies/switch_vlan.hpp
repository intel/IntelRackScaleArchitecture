/*!
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
 *
 * @file api/ies/switch_vlan.hpp
 *
 * @brief IES Vlan functionality.
 * */

#ifndef AGENT_NETWORK_IES_SWITCH_VLAN_HPP
#define AGENT_NETWORK_IES_SWITCH_VLAN_HPP

#include "network_config.hpp"
#include "api/switch_vlan.hpp"

#ifdef IES_FOUND
#if __cplusplus >= 201103L
extern "C"{
#endif
#include <fm_sdk.h>
#include <platform_api.h>
#if __cplusplus >= 201103L
}
#endif

using std::uint8_t;
using std::uint32_t;
using std::string;
using std::to_string;
using std::runtime_error;

namespace agent {
namespace network {
namespace api {
namespace ies {

/*! IES Switch Vlan class */
class SwitchVlan: public api::SwitchVlan {
public:

    /*!
     * @brief Default constructor.
     * @param[in] switch_uuid Switch UUID.
     * @param[in] vlan Vlan identifier.
     */
    SwitchVlan(const std::string& switch_uuid, const std::string& vlan);

    /*!
     * @brief Default destructor.
     */
    virtual ~SwitchVlan() {}

    /*!
     * @brief Check if port is present in the vlan.
     * @param[in] port Port identifier.
     * @return Status.
     */
    bool is_port_present(const std::string& port);

    /*!
     * @brief Gets vlan port parameters
     * @param[in] switch_id Switch id.
     * @param[in] vlan_id Vlan id.
     * @return Status.
     */
    static bool get_switch_vlan_is_present(fm_int switch_id, fm_int vlan_id);

    /*!
     * @brief Gets vlan port parameters
     * @param[in] switch_id Switch id.
     * @param[in] vlan_id Vlan id.
     * @param[in] port_id Port id.
     * @return Status.
     */
    static bool get_switch_vlan_port_is_present(fm_int switch_id, fm_int vlan_id, fm_int port_id);

    /*!
     * @brief Reads port vlan ids
     * @param[in] switch_id Switch id.
     * @param[in] port_id Port id.
     * @return Vlan list.
     */
    static VlanList read_switch_port_vlan_list(SwitchId switch_id, PortId port_id);

    /*!
     * @brief Writes switch vlan attribute
     * @param[in] switch_id Switch id.
     * @param[in] vlan_id Vlan id.
     * @param[in] attr Attribute type.
     * @param[in] attr_value_ptr Pointer to attribute value.
     */
    static void write_switch_vlan_attribute(
        SwitchId switch_id, VlanId vlan_id, fm_int attr, void* attr_value_ptr);

    /*!
     * @brief Creates switch vlan
     * @param[in] switch_id Switch id.
     * @param[in] vlan_id Vlan id.
     * @param[out] vlan_created_ptr Pointer to flag - vlan created or not.
     */
    static void create_switch_vlan(
        SwitchId switch_id, fm_uint16 vlan_id, bool* vlan_created_ptr);

    /*!
     * @brief Reads vlan port parameters
     * @param[in] switch_id Switch id.
     * @param[in] vlan_id Vlan id.
     * @param[in] port_id Port id.
     * @param[out] tag_value_ptr Pointer to tagged flag
     */
    static void read_switch_vlan_port_tag(
        fm_int switch_id, fm_int vlan_id, fm_int port_id, fm_bool* tag_value_ptr);

    /*!
     * @brief Reads vlan port parameters
     * @param[in] switch_id Switch id.
     * @param[in] vlan_id Vlan id.
     * @param[in] port_id Port id.
     * @param[out] is_member_ptr Pointer to member flag
     * @param[out] is_pvid_ptr Pointer to pvid flag
     * @param[out] is_tagged_ptr Pointer to tagged flag
     * @return Status.
     */
    static fm_status read_vlan_port_params(
        SwitchId switch_id, VlanId vlan_id, PortId port_id,
        bool *is_member_ptr, bool *is_pvid_ptr, bool *is_tagged_ptr);

    /*!
     * @brief Add port cleanup.
     * @param[in] switch_id Switch id.
     * @param[in] vlan_id Vlan id.
     * @param[in] port_id Port id.
     * @param[in] vlan_port_added Vlan port added flag.
     * @param[in] vlan_created Vlan created flag.
     */
    [[ noreturn ]] static void add_port_cleanup_and_throw(
        SwitchId switch_id, VlanId vlan_id, PortId port_id,
        bool vlan_port_added, bool vlan_created);

    /*!
     * @brief Add vlan.
     */
    void add_vlan() {}

    /*!
     * @brief Remove vlan.
     */
    void remove_vlan() {}

    /*!
     * @brief Add port to vlan.
     * @param[in] port Port identifier.
     * @param[in] tagged Vlan tag(false/true).
     */
    void add_port(const PortIdentifier& port, bool tagged);

    /*!
     * @brief Remove port from vlan.
     * @param[in] port Port identifier.
     */
    void remove_port(const PortIdentifier& port);

    /*!
     * @brief Get vlan port information.
     * @param[in] port Port identifier.
     * @param[out] info Vlan port information.
     */
    void get_info(const PortIdentifier& port, VlanPortInfo& info);
};

}
}
}
}
#endif /* IES_FOUND */
#endif /* AGENT_NETWORK_IES_SWITCH_VLAN_HPP */
