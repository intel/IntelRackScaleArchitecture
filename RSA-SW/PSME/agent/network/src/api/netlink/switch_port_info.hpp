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
 * @file api/netlink/switch_port_info.hpp
 *
 * @brief Switch port parameters
 * */

#ifndef AGENT_NETWORK_NETLINK_GET_SWITCH_PORT_INFO_HPP
#define	AGENT_NETWORK_NETLINK_GET_SWITCH_PORT_INFO_HPP

#include "api/switch_port_info.hpp"

namespace agent {
namespace network {
namespace api {
namespace netlink {

/*!
 * @brief Switch Port Info.
 */
class SwitchPortInfo : public api::SwitchPortInfo {
public:
    /*!
     * @brief Default constructor.
     * @param[in] uuid Switch UUID.
     * @param[in] port_identifier Port identifier.
     */
    SwitchPortInfo(const std::string& uuid, const std::string& port_identifier);

    /*!
     * @brief Default constructor.
     * @param[in] port_identifier Port identifier.
     */
    SwitchPortInfo(const std::string& port_identifier);

    /*!
     * @brief Copy constructor.
     * @param[in] orig object to be copied.
     */
    SwitchPortInfo(const SwitchPortInfo& orig) = delete;

    /*!
     * @brief Copy operator
     * @param[in] orig object to be copied.
     */
    SwitchPortInfo& operator=(const SwitchPortInfo &orig) = delete;

    /*!
     * @brief Default destructor.
     */
    virtual ~SwitchPortInfo();

    /*!
     * @brief Gets switch port link state thru Netlink api.
     */
    void get_switch_port_link_state();

    /*!
     * @brief Sets switch port link state thru Netlink api.
     */
    void set_switch_port_link_state();

    /*!
     * @brief Gets port speed from a switch thru Netlink api.
     * @return Port speed.
     */
    uint32_t get_switch_port_speed();

    /*!
     * @brief Sets port speed on a switch thru Netlink api.
     * @param[in] speed Port speed.
     */
    void set_switch_port_speed(uint32_t speed);

    /*!
     * @brief Gets port attribute from a switch.
     * @param[in] attr Port attribute to get.
     * @param[out] value Return value.
     */
    void get_switch_port_attribute(PortAttribute attr,
                                   PortAttributeValue& value);

    /*!
     * @brief Sets port attribute on a switch.
     * @param[in] attr Port attribute to set.
     * @param[in] value Value to set.
     */
    void set_switch_port_attribute(PortAttribute attr,
                                   const PortAttributeValue& value);

private:

    using SwAttr = uint32_t;
    using SwAttrValue = uint32_t;

    /*!
     * @brief Sets port attribute on a switch thu Netlink.
     * @param[in] swattr Port attribute to set.
     * @param[in] value Value to set.
     */
    void set_switch_port_attr(SwAttr swattr, SwAttrValue value);

    /*!
     * @brief Get switch port max frame size attribute.
     * @return Port mac frame size.
     */
    uint32_t get_switch_port_max_frame_size();

    /*!
     * @brief Get switch port autoneg attribute.
     * @return Port autoneg (true/false).
     */
    bool get_switch_port_autoneg();

    /*!
     * @brief Get vlan list on the port.
     */
    void get_switch_vlan_list();

    /*!
     * @brief Gets host MAC address on a port from a switch thru Netlink api.
     * @return Port MAC address.
     */
    string get_switch_port_mac_address();

    /* IFLA_SWPORT_ATTRS and AFLA_ATTR_* definitions are
     * in if_link.h linux header file of NOS linux image.
     * Temporary define them here. */
    static constexpr uint16_t IFLA_ATTR_AUTONEG = 24;
    static constexpr uint16_t IFLA_ATTR_MAX_FRAME_SIZE = 52;
};

}
}
}
}
#endif	/* AGENT_NETWORK_NETLINK_SWITCH_PORT_INFO_HPP */
