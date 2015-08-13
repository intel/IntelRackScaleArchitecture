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
 * @file message.cpp
 *
 * @brief Switch port parameters
 * */

#include "message.hpp"
#include "logger/logger_factory.hpp"

#include <cstdio>
#include <sstream>
#include <iomanip>
#include <memory>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/if_bridge.h>


using namespace agent::network::api::netlink;

struct sw_attributes {
    __u64 attribute_val;     /* Value of the attribute*/
    __s64 attribute_index;   /* Attribute index */
};

Message::Message() {}

Message::~Message() {}

bool Message::prepare_getneigh_msg(Message::Pointer &msg) {
    struct ndmsg ndm{};
    auto port = create_port_identifier(m_sw, m_port);
    m_ifindex = (int) if_nametoindex(port.c_str());

    ndm.ndm_family = PF_BRIDGE;
    ndm.ndm_ifindex = m_ifindex;
    if (nlmsg_append(msg.get(), &ndm, sizeof(ndm), NLMSG_ALIGNTO) < 0) {
        return false;
    }

    return true;
}

Message::Pointer Message::prepare_rtnl_msg() {
    struct ifinfomsg ifi{};
    Pointer msg(nlmsg_alloc_simple(m_type, int(m_flags)));

    if (!msg) {
        return nullptr;
    }

    if (RTM_GETNEIGH == m_type) {
        if (prepare_getneigh_msg(msg)) {
            return msg;
        } else {
            return nullptr;
        }
    }

    ifi.ifi_family = AF_UNSPEC;
    if (SwitchPortInfo::State::UNKNOWN != m_link_state) {
        /* prepare message for SET operation */
        ifi.ifi_type = ARPHRD_ETHER;
        ifi.ifi_change |= IFF_UP;
        if (SwitchPortInfo::State::UP == m_link_state) {
            ifi.ifi_flags = IFF_UP | IFF_RUNNING;
        }
        else {
            ifi.ifi_flags &= static_cast<unsigned int>(~IFF_UP);
        }
    }
    /* Appending the ndm parameter details to netlink message msg */
    if (nlmsg_append(msg.get(), &ifi, sizeof(ifi), NLMSG_ALIGNTO) < 0) {
        return nullptr;
    }

    auto str = create_port_identifier(m_sw, m_port);
    if (nla_put(msg.get(), IFLA_IFNAME, int((str.length())), str.c_str()) < 0) {
        return nullptr;
    }

    log_debug(GET_LOGGER("netlink"), "netlink message created for device " + str);
    return msg;
}

Message::Pointer Message::prepare_switch_msg() const {
    struct ifinfomsg ifi{};
    struct sw_attributes attr_val_index{};
    struct nlattr *port_config_attr;
    Pointer msg(nlmsg_alloc_simple(m_type, int(m_flags)));
    if (!msg) {
        return nullptr;
    }

    /* Appending the ndm parameter details to netlink message msg */
    if (nlmsg_append(msg.get(), &ifi, sizeof(ifi), NLMSG_ALIGNTO) < 0) {
        return nullptr;
    }

    /* add iface name into the message */
    auto str = create_port_identifier(m_sw, m_port);
    if (nla_put(msg.get(), IFLA_IFNAME, int((str.length())), str.c_str()) < 0) {
        return nullptr;
    }

    /* set port attribute into the message */
    port_config_attr = nla_nest_start(msg.get(), IFLA_SWPORT_ATTRS);
    if (!port_config_attr) {
        return nullptr;
    }
    attr_val_index.attribute_index = -1;
    attr_val_index.attribute_val = m_swattr_value;
    if (nla_put(msg.get(), int(m_swattr), sizeof(attr_val_index),
        &attr_val_index) < 0) {
        return nullptr;
    }
    nla_nest_end(msg.get(), port_config_attr);

    log_debug(GET_LOGGER("netlink"), "netlink switch message created for device " + str);
    return msg;
}

string Message::create_port_identifier(Switch sw, Port port) {
    std::stringstream ifname;
    ifname << "sw" << int(sw) << "p" << int(port);
    return ifname.str();
}

void Message::set_port(const PortIdentifier& port) {
    Switch sw_id{0};
    Port sw_port{0};
    std::string sw_prefix{};
    char port_prefix{};
    std::istringstream stream(port);

    /* Port identifier format: sw<Switch>p<Port> */
    stream.width(2);       /* set switch prefix length */
    stream >> sw_prefix;   /* read switch prefix */
    stream >> sw_id;       /* get switch index */
    stream >> port_prefix; /* read port prefix */
    stream >> sw_port;     /* get switch port index */
    if (!stream.eof() || stream.fail() ||
        "sw" != sw_prefix || 'p' != port_prefix) {
        throw std::runtime_error("set_port() wrong port identifier.");
    }

    set_switch(Switch(sw_id));
    set_port(Port(sw_port));
}

void Message::parse_attr(struct rtattr *tb[], unsigned int max,
                         struct rtattr *rta, int len)
{
    while (RTA_OK(rta, len)) {
        unsigned short type = rta->rta_type;
        if ((type <= max) && (nullptr == tb[type])) {
            tb[type] = rta;
        }
        rta = RTA_NEXT(rta,len);
    }
}

#define RTA(r) ((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct ndmsg))))

int Message::read_neighbour(struct nl_msg *nlmsg, void *arg) {
    struct nlmsghdr *hdr;
    struct rtattr *tb[NDA_MAX + 1]{};
    struct ndmsg *data;
    Message *message = (Message*) arg;

    if (nullptr == nlmsg) {
        return NL_OK;
    }

    if (nullptr == message) {
        return NL_OK;
    }

    hdr = nlmsg_hdr(nlmsg);
    data = static_cast<struct ndmsg*> (nlmsg_data(hdr));

    if (hdr->nlmsg_type == NLMSG_DONE) {
        message->set_last();
        return NL_OK;
    }

    if (hdr->nlmsg_type != RTM_NEWNEIGH) {
        return NL_OK;
    }

    if (data->ndm_family != AF_BRIDGE) {
        return NL_OK;
    }

    if (message->get_ifindex() != data->ndm_ifindex) {
        return NL_OK;
    }

    parse_attr(tb, NDA_MAX, RTA(data),
               (int) hdr->nlmsg_len - (int) NLMSG_LENGTH(sizeof(*data)));

    if (tb[NDA_LLADDR] && (0 == message->get_address().length())) {
        unsigned char *addr = static_cast<unsigned char *>
            (RTA_DATA(tb[NDA_LLADDR]));
        std::stringstream ss;
        const char *sep = "";
        ss.fill('0');
        for (unsigned i = 0; i < ETH_ALEN; ++i) {
            ss << sep;
            sep = ":";
            ss << std::uppercase << std::setw(2) << std::hex <<
                static_cast<unsigned int>(addr[i]);
        }

        message->set_address(ss.str());
    }

    return NL_OK;
}

