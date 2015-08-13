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
 * @file socket.hpp
 *
 * @brief Netlink socket class
 * */

#ifndef AGENT_NETWORK_NETLINK_SOCKET_HPP
#define AGENT_NETWORK_NETLINK_SOCKET_HPP

#include "message.hpp"

#include <memory>

/*! forward declarations of types that are used */
struct nl_sock;

namespace agent {
namespace network {
namespace api {
namespace netlink {

/*!
 * @brief Netlink socket.
 */
class Socket {
public:
    /*!
     * @brief Default constructor.
     */
    Socket();

    /*!
     * @brief Copy constructor.
     * @param orig object to be copied.
     */
    Socket(const Socket& orig) = delete;

    /*!
     * @brief Copy operator.
     * @param[in] s object to be copied
     */
    Socket& operator= (const Socket& s) = delete;

    /*!
     * @brief Default destructor.
     */
    virtual ~Socket();

    /*!
     * @brief Connect socket.
     */
    void connect();

    /*!
     * @brief Send a message through the socket.
     * @param[in] message Message to be sent.
     */
    void send_message(const Message& message);

    /*!
     * @brief Send a message through the socket.
     * @param[in] msg Message to be sent.
     */
    void send(Message& msg);

    /*!
     * @brief Receive message from socket.
     * @param[in] message Message to be received
     */
    void receive(Message& message);

    /*!
     * @brief Receive message from socket.
     * @param[out] message Message to be received
     */
    void receive_message(Message &message);

    /*!
     * @brief Send a switch attribute message through the socket.
     * @param[in] message Message to be sent.
     */
    void send_switch(const Message& message);

    /*!
     * @brief Set a custom callback that is called on each received message.
     * @param[in] func function to be called.
     * @param[in] arg agrument that will be passed to that function.
     * @return true if the callback function was set properly, false otherwise.
     */
    bool modify_callback(nl_recvmsg_msg_cb_t func, void *arg);

    /*!
     * @brief Receive messages from socket and call the callback that was set.
     * @param[out] message Message to be received
     */
    void receive_to_callback(Message &message);

private:
    struct Deleter {
        void operator()(struct nl_sock* sock) const {
            nl_socket_free(sock);
        }
    };

    using Pointer = std::unique_ptr<struct nl_sock, Deleter>;
    Pointer m_sock;

    static constexpr uint16_t RECV_MESSAGE_SIZE = 16384;
    static constexpr uint16_t ERR_MESSAGE_SIZE = 256;

    /*!
     * @brief Convert error number into string.
     * @param[in] errnum Error number.
     * @return String describing the error number.
     */
    std::string error_to_string(int errnum);
};

}
}
}
}

#endif /* AGENT_NETWORK_NETLINK_SOCKET_HPP */
