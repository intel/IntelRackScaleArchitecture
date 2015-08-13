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
 * @file vlan_message.hpp
 *
 * @brief Switch vlan configuration
 * */

#ifndef AGENT_NETWORK_NETLINK_VLAN_MESSAGE_HPP
#define AGENT_NETWORK_NETLINK_VLAN_MESSAGE_HPP

#include "message.hpp"

#include <memory>
#include <map>

namespace agent {
namespace network {
namespace api {
namespace netlink {

/*! Vlan Port Message */
class VlanPortMessage: public Message {
public:
    /*!
     * @brief Default constructor.
     * @param[in] port Port index.
     * @param[in] vlan Vlan index.
     * @param[in] tagged Vlan tag.
     */
    VlanPortMessage(Port port, Vlan vlan, bool tagged);

    /*!
     * @brief Default destructor.
     */
    virtual ~VlanPortMessage();

    /*!
     * @brief Set vlan index.
     * @param[in] vlan Vlan index.
     */
    void set_vlan_id(Vlan vlan) { m_vlan_id = vlan; }

    /*!
     * @brief Get vlan index.
     * @return Vlan index.
     */
    Vlan get_vlan_id() const { return m_vlan_id; }

    /*!
     * @brief Set vlan tag.
     * @param[in] tag Vlan tag (true/false).
     */
    void set_tagged(bool tag) { m_tagged = tag; }

    /*!
     * @brief Get vlan tag.
     * @return Vlan tag (true/false).
     */
    bool get_tagged() const { return m_tagged; }

private:

    Vlan m_vlan_id;
    bool m_tagged;
};

/*! Add Vlan Port Message */
class AddVlanPortMessage: public VlanPortMessage {
public:

    /*!
     * @brief Default constructor.
     * @param[in] port Port index.
     * @param[in] vlan Vlan index.
     * @param[in] tagged Vlan tag.
     */
    AddVlanPortMessage(Port port, Vlan vlan, bool tagged);

    /*!
     * @brief Default destructor.
     */
    virtual ~AddVlanPortMessage();

    /*!
     * @brief Prepare add vlan port netlink message.
     * @return Netlink message ready to be sent.
     */
    Pointer prepare_netlink_msg() const;
};

/*! Delete Vlan Port Message */
class DelVlanPortMessage: public AddVlanPortMessage {
public:

    /*!
     * @brief Default constructor.
     * @param[in] port Port index.
     * @param[in] vlan Vlan index.
     */
    DelVlanPortMessage(Port port, Vlan vlan);

    /*!
     * @brief Default destructor.
     */
    virtual ~DelVlanPortMessage();
};

/*! Info Vlan Port Message */
class InfoVlanPortMessage: public VlanPortMessage {
public:

    /*! Vlan information */
    struct VlanInfo {
        Vlan m_vlan_id{};
        bool m_tagged{};
        bool m_pvid{};
    };

    using VlanInfoList = std::vector<VlanInfo>;
    using VlanPortInfoMap = std::map<PortIdentifier, VlanInfoList>;

    /*!
     * @brief Default constructor.
     */
    InfoVlanPortMessage();

    /*!
     * @brief Prepare info vlan port netlink message.
     * @return Netlink message ready to be sent.
     */
    Pointer prepare_netlink_msg() const;

    /*!
     * @brief check if the port is vlan tagged.
     * @param[in] port Port identifier.
     * @param[in] vlan Vlan index.
     * @return true - tagged, false - untagged.
     */
    bool is_tagged(const PortIdentifier& port, Vlan vlan);

    /*!
     * @brief Get vlan list on a port.
     * @param[in] port Port index.
     * @return Vlan list on the port.
     */
    VlanInfoList get_vlan_list(Port port) const;

    /*!
     * @brief Parse recv netlink header.
     * @param[in] nlhdr Netlink header.
     */
    void parse_netlink_hdr(NetlinkHeader& nlhdr);

private:
    VlanPortInfoMap m_vlan_port_info{};
};

}
}
}
}

#endif /* AGENT_NETWORK_NETLINK_VLAN_MESSAGE_HPP */
