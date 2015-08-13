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
 * @file message.hpp
 *
 * @brief Switch port parameters
 * */

#ifndef AGENT_NETWORK_NETLINK_MESSAGE_HPP
#define AGENT_NETWORK_NETLINK_MESSAGE_HPP

#include "switch_port_info.hpp"

#include <netlink/genl/genl.h>

#include <memory>
#include <map>

namespace agent {
namespace network {
namespace api {
namespace netlink {

/*!
 * @brief Netlink socket.
 */
class Message {
public:
    struct Deleter {
        void operator()(struct nl_msg* msg) const {
            nlmsg_free(msg);
        }
    };

    using Pointer = std::unique_ptr<struct nl_msg, Deleter>;
    using Vlan = uint16_t;
    using Type = uint16_t;
    using Flags = uint32_t;
    using Switch = uint16_t;
    using Port = uint16_t;
    using SwAttr = uint32_t;
    using SwAttrValue = uint32_t;
    using PortIdentifier = std::string;

    /*! Netlink header class */
    class NetlinkHeader {
    private:
        struct nlmsghdr* p_hdr;
    public:
        /*!
         * @brief Default constructor.
         */
        NetlinkHeader() = default;

        /*!
         * @brief Default constructor.
         * @param[in] p Pointer to netlink message.
         */
        NetlinkHeader(struct nlmsghdr* p) : p_hdr(p) { }

        /*!
         * @brief Get netlink data.
         * @return Netlink data.
         */
        void* get_data() const { return nlmsg_data(p_hdr); }

        /*!
         * @brief Get netlink message length.
         * @return Netlink message length.
         */
         __u32 get_len() const { return p_hdr->nlmsg_len; }

        /*!
         * @brief Get netlink message type.
         * @return Netlink message type.
         */
         __u16 get_type() const { return p_hdr->nlmsg_type; }
    };

    /*!
     * @brief Default constructor.
     */
    Message();

    /*!
     * @brief Default destructor.
     */
    virtual ~Message();

    /*!
     * @brief Copy constructor.
     * @param orig object to be copied.
     */
    Message(const Message& orig) = delete;

    /*!
     * @brief Copy operator.
     * @param s object to be copied
     */
    Message& operator= (const Message& s) = delete;

    /*!
     * @brief Set type of message.
     * @param type Netlink message type to be set.
     */
    void set_type(Type type) { m_type = type; }

    /*!
     * @brief Set flags of message
     * @param flags Netlink flags to be set.
     */
    void set_flags(Flags flags) { m_flags = flags; }

    /*!
     * @brief Set switch number.
     * @param sw Switch number.
     */
    void set_switch(Switch sw) { m_sw = sw; }

    /*!
     * @brief Set port number.
     * @param port Port number.
     */
    void set_port(Port port) { m_port = port; }

    /*!
     * @brief Set port identifier.
     * @param port Port identifier.
     */
    void set_port(const PortIdentifier& port);

    /*!
     * @brief Set port attribute.
     * @param[in] attr Switch port attribute.
     * @param[in] value Switch port attribute value.
     */
    void set_switch_attr(SwAttr attr, SwAttrValue value) {
        m_swattr = attr;
        m_swattr_value = value;
    }

    /*!
     * @brief Set port state.
     * @param state Port state.
     */
    void set_link_state(SwitchPortInfo::State state) {
        m_link_state = state;
    }

    /*!
     * @brief Get port state.
     * @return state Port state.
     */
    SwitchPortInfo::State get_link_state() { return m_link_state; }

    /*!
     * @brief Set operational port state.
     * @param state Operational port state.
     */
    void set_operational_state(SwitchPortInfo::State state) {
        m_operational_state = state;
    }

    /*!
     * @brief Get operational port state.
     * @return state Operational port state.
     */
    SwitchPortInfo::State get_operational_state() const {
        return m_operational_state;
    }

    /*!
     * @brief Prepare rtnl message to be sent.
     * @return Netlink message ready to be sent.
     */
    Pointer prepare_rtnl_msg();

    /*!
     * @brief Prepare netlink switch message to be sent.
     * @return Netlink message ready to be sent.
     */
    Pointer prepare_switch_msg() const;

    /*!
     * @brief Create port identifier for given switch and port.
     * @param sw switch index.
     * @param port port index.
     * @return string port identifier.
     */
    static string create_port_identifier(Switch sw, Port port);

    /*!
     * @brief Prepare netlink message to be sent.
     * @return Netlink message ready to be sent.
     */
    virtual Pointer prepare_netlink_msg() const {
        return nullptr;
    }

    /*!
     * @brief Parse recv netlink header.
     * @param[in] nlhdr Netlink header.
     */
    virtual void parse_netlink_hdr(NetlinkHeader& nlhdr) { (void)nlhdr; }

    /*!
     * @brief Check if the message is the last in the series of messages.
     * @return true if the message is last, false otherwise.
     */
    bool get_last() { return m_last; }

    /*!
     * @brief Set the message as the last in the series of messages.
     */
    void set_last() { m_last = true; }

    /*!
     * @brief Get interface index.
     * @return interface index.
     */
    int get_ifindex() { return m_ifindex; }

    /*!
     * @brief set MAC address.
     * @param str MAC address to be set.
     */
    void set_address(const string& str) { m_address = str; }

    /*!
     * @brief get MAC address.
     * @return MAC address.
     */
    string& get_address() { return m_address; }

    /*!
     * @brief Read neighbour from the netlink message.
     * @return returns NL_OK.
     */
    static int read_neighbour(struct nl_msg *nlmsg, void *arg);

protected:
    SwAttr m_swattr{};
    SwAttrValue m_swattr_value{};
    Type m_type{RTM_GETLINK};
    Flags m_flags{};
    Switch m_sw{};
    Port m_port{};
    SwitchPortInfo::State m_link_state{SwitchPortInfo::State::UNKNOWN};
    static constexpr uint16_t ARPHRD_ETHER = 1;
    SwitchPortInfo::State m_operational_state{SwitchPortInfo::State::UNKNOWN};
    bool m_last{false};
    int m_ifindex{};
    string m_address{};

    /* IFLA_SWPORT_ATTRS and AFLA_ATTR_* definitions are
     * in if_link.h linux header file of NOS linux image.
     * Temporary define them here. */
    static constexpr uint16_t IFLA_SWPORT_ATTRS = 38;

private:
    /*!
     * @brief Prepare getneigh rtnl message to be sent.
     * @param msg already allocated message.
     * @return true if message is ready to be sent, false otherwise.
     */
    bool prepare_getneigh_msg(Pointer &msg);

    /*!
     * @brief parse netlink attributes.
     * @param tb the resulting table with attributes.
     * @param max maximum number of elements in tb.
     * @param rta the attributes as received from the network.
     * @param len the number of attributes in rta.
     */
    static void parse_attr(struct rtattr *tb[], unsigned int max,
                           struct rtattr *rta, int len);
};

}
}
}
}

#endif /* AGENT_NETWORK_NETLINK_MESSAGE_HPP */
