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
 * @file socket.cpp
 *
 * @brief Netlink socket implementation.
 * */

#include "socket.hpp"
#include "logger/logger_factory.hpp"

#include <sys/socket.h>
#include <linux/netdevice.h>
#include <netlink/genl/genl.h>
#include <exception>
#include <memory>

using namespace agent::network::api::netlink;

static const int BUFLEN = 4096;
struct rcvbuf_t {
    struct nlmsghdr hdr;
    struct ifinfomsg ifi;
    char data[BUFLEN];
};

Socket::Socket() : m_sock(nl_socket_alloc()) {}

Socket::~Socket() {}

void Socket::connect() {
    if (!m_sock) {
        throw std::runtime_error("null socket");
    }

    /* binds and connects to the socket of NETLINK_ROUTE protocol */
    if (nl_connect(m_sock.get(), NETLINK_ROUTE) != 0) {
        throw std::runtime_error("nl_connect failed");
    }
}

void Socket::send_message(const Message& message) {
    auto nlmsg = message.prepare_netlink_msg();
    if (nullptr == nlmsg) {
        throw std::runtime_error("null netlink message");
    }
    if (0 > nl_send_auto(m_sock.get(), nlmsg.get())) {
        throw std::runtime_error("nl_send_auto failed");
    }
}

void Socket::send(Message& message) {
    auto nlmsg = message.prepare_rtnl_msg();
    if (nullptr == nlmsg) {
        throw std::runtime_error("null netlink message");
    }
    if (0 > nl_send_auto(m_sock.get(), nlmsg.get())) {
        throw std::runtime_error("nl_send_auto failed");
    }
}

void Socket::send_switch(const Message& message) {
    auto nlmsg = message.prepare_switch_msg();
    if (nullptr == nlmsg) {
        throw std::runtime_error("null netlink message");
    }
    if (0 > nl_send_auto(m_sock.get(), nlmsg.get())) {
        throw std::runtime_error("send failed when sending switch message");
    }
}

void Socket::receive(Message &message) {
    struct rcvbuf_t rcvbuf{};
    int sockfd;
    ssize_t nLen;

    sockfd = nl_socket_get_fd(m_sock.get());
    while ((nLen = recv(sockfd, &rcvbuf, sizeof(rcvbuf), 0)) > 0) {
        const auto& nlh = &rcvbuf.hdr;
        if (NLMSG_OK(nlh, nLen)) {
            if (NLMSG_ERROR == nlh->nlmsg_type) {
                throw std::runtime_error("error on receive");
            }
            if (rcvbuf.ifi.ifi_flags & IFF_UP) {
                message.set_link_state(SwitchPortInfo::State::UP);
            }
            else {
                message.set_link_state(SwitchPortInfo::State::DOWN);
            }
            if (rcvbuf.ifi.ifi_flags & IFF_RUNNING) {
                message.set_operational_state(SwitchPortInfo::State::UP);
            }
            else {
                message.set_operational_state(SwitchPortInfo::State::DOWN);
            }
            break;
        }
    }
    if (nLen <= 0) {
        throw std::runtime_error("recv failed");
    }
}

void Socket::receive_message(Message &message) {
    std::unique_ptr<uint8_t[]> data(new uint8_t[RECV_MESSAGE_SIZE]);
    struct sockaddr_nl nladdr{};
    struct iovec iov{};
    struct msghdr msg{};
    int sockfd{-1};
    ssize_t status{-1};
    int msglen{0};
    struct nlmsghdr* nl_header = nullptr;

    msg.msg_name = &nladdr;
    msg.msg_namelen = sizeof(nladdr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1; /* only one message */
    iov.iov_base = data.get();

    /* receive message loop */
    sockfd = nl_socket_get_fd(m_sock.get());
    while (true) {
        iov.iov_len = RECV_MESSAGE_SIZE;
        status = recvmsg(sockfd, &msg, 0);
        if (0 > status) {
            if (EINTR == errno || EAGAIN == errno) {
                continue;
            }
            throw std::runtime_error("recvmsg() failed");
        }

        if (0 == status) {
            throw std::runtime_error("recvmsg() EOF on netlink");
        }

        nl_header = reinterpret_cast<struct nlmsghdr*>(data.get());
        msglen = int(status);

        while (nlmsg_ok(nl_header, msglen)) {
            if (0 == nladdr.nl_pid) {
                if (nl_header->nlmsg_flags & NLM_F_DUMP_INTR) {
                    log_debug(GET_LOGGER("netlink"),
                              "Dump was interrupted and may be inconsistent");
                }
                if (nl_header->nlmsg_type == NLMSG_DONE) {
                    return;
                }
                if (nl_header->nlmsg_type == NLMSG_ERROR) {
                    struct nlmsgerr* err = static_cast<struct nlmsgerr*>
                                                       (nlmsg_data(nl_header));
                    if (int(nl_header->nlmsg_len) <
                        nlmsg_size(sizeof(struct nlmsgerr))) {
                        throw std::runtime_error("Netlink message truncated");
                    }
                    else {
                        throw std::runtime_error(
                                    std::string("Netlink message error: ")
                                    + error_to_string(abs(err->error))
                                    + " (" + std::to_string(abs(err->error))
                                    + ")");
                    }
                }

                /* parse netlink header */
                Message::NetlinkHeader nlhdr(nl_header);
                message.parse_netlink_hdr(nlhdr);
            }
            nl_header = nlmsg_next(nl_header, &msglen);
        }

        if (msg.msg_flags & MSG_TRUNC) {
            log_debug(GET_LOGGER("netlink"), "Message truncated");
        }
        if (msglen) {
            log_debug(GET_LOGGER("netlink"), "Remnant of size");
        }
    }
}

std::string Socket::error_to_string(int errnum) {
    char err_buffer[ERR_MESSAGE_SIZE]{};
#if ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE)
    strerror_r(errnum, err_buffer, sizeof(err_buffer));
    char* err_msg = err_buffer;
#else
    char* err_msg = strerror_r(errnum, err_buffer, sizeof(err_buffer));
#endif
    return std::string(err_msg);
}

bool Socket::modify_callback(nl_recvmsg_msg_cb_t func, void *arg) {
    int ret;

    ret = nl_socket_modify_cb(m_sock.get(), NL_CB_VALID, NL_CB_CUSTOM, func,
                              arg);
    if (0 != ret)
        return false;
    ret = nl_socket_modify_cb(m_sock.get(), NL_CB_FINISH, NL_CB_CUSTOM, func,
                              arg);
    if (0 != ret)
        return false;
    return true;
}

void Socket::receive_to_callback(Message &message) {
    while(!message.get_last()) {
        auto err = nl_recvmsgs_default(m_sock.get());
        if (0 > err) {
            break;
        }
    }
}

