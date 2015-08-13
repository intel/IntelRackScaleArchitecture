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
 */

#include "remote_switch_info.hpp"

#include "hw/fm10000/network_controller_manager.hpp"
#include "hw/fm10000/network_controller.hpp"

#include <safe-string/safe_lib.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <unistd.h>
#include <cstring>
#include <exception>
#include <sstream>


static const unsigned HOP_MAX_PATH = 256;
static const unsigned MAX_HOPS = 1024;

#define ATTRIBUTE_PACKED __attribute__((packed))

struct switch_info {
    unsigned char mac[ETH_ALEN];
    struct in_addr ipv4addr;
    int32_t metric;
    uint32_t num_nexthop;
    struct {
        uint32_t type;
        char path[HOP_MAX_PATH];
    } nexthops[MAX_HOPS];
} ATTRIBUTE_PACKED;


using namespace agent::network::api::dcrp;
using namespace agent::network::hw::fm10000;

RemoteSwitchInfo::RemoteSwitchInfo(const std::string& uuid) {
    auto controller = NetworkControllerManager::get_network_controller();
    controller->get_switch_id(uuid);
}

RemoteSwitchInfo::~RemoteSwitchInfo() {}

void RemoteSwitchInfo::get_known_switches_id(vector<string>& switches_id) {
    Pointer buffer(nullptr);
    uint32_t bsize = 0;
    uint32_t count = 0;

    auto status = processCommand("GetNodes", buffer, bsize);
    if (status) {
        throw std::runtime_error("GetNodes ID command failed on remote side.");
    }
    if (sizeof(count) > bsize) {
        throw std::runtime_error("GetNodes ID command result is unexpected");
    }
    auto ptr = buffer.get();
    memcpy_s(&count, sizeof(count), ptr, sizeof(count));
    ptr += sizeof(count);
    while (count-- > 0) {
        switches_id.push_back(ptr);
        ptr += strnlen_s(ptr, RSIZE_MAX_STR) + 1;
    }
}

void RemoteSwitchInfo::get_neighbor_switches_id(const string& id,
                                                vector<string>& switches_id) {
    Pointer buffer(nullptr);
    uint32_t bsize = 0;
    uint32_t count = 0;

    auto status = processCommand("GetNodeNeighbors " + id, buffer, bsize);
    if (status) {
        throw std::runtime_error("GetNodeNeighbors command failed on remote side.");
    }
    if (sizeof(count) > bsize) {
        throw std::runtime_error("GetNodeNeighbors command result is unexpected");
    }
    auto ptr = buffer.get();
    memcpy_s(&count, sizeof(count), ptr, sizeof(count));
    ptr += sizeof(count);
    while (count-- > 0) {
        switches_id.push_back(ptr);
        ptr += strnlen_s(ptr, RSIZE_MAX_STR) + 1;
    }
}

void RemoteSwitchInfo::get_switch_info(const string& id, Response& response) {
    Pointer buffer(nullptr);
    uint32_t bsize = 0;
    switch_info sinfo{};
    static const char CHARS_PER_OCTET = 3;
    char mac[ETH_ALEN * CHARS_PER_OCTET];

    auto status = processCommand("GetNodeDetails " + id, buffer, bsize);
    if (status) {
        throw std::runtime_error("GetNodeDetails command failed on remote side.");
    }
    if (sizeof(switch_info) < bsize) {
        throw std::runtime_error("GetNodeDetails command result is unexpected");
    }
    memcpy_s(&sinfo, sizeof(sinfo), buffer.get(), bsize);

    std::snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X",
                  sinfo.mac[0], sinfo.mac[1], sinfo.mac[2],
                  sinfo.mac[3], sinfo.mac[4], sinfo.mac[5]);
    response.set_mac_address(mac);

    for (uint32_t i = 0; i < sinfo.num_nexthop; i++) {
        // TODO current DCRP implementation returns port only not supporting IPs
	// also metric is the same for all paths
        response.add_next_hop({sinfo.nexthops[i].path, "", "", sinfo.metric});
    }
}

void RemoteSwitchInfo::writeInt(int sock_id, uint32_t value) {
    value = htonl(value);
    if (write(sock_id, &value, sizeof(value)) !=  sizeof(value)) {
        throw std::runtime_error("failed writing int to socket");
    }
}

void RemoteSwitchInfo::readInt(int sock, uint32_t& i) {
    if (read(sock, &i, sizeof(int)) != sizeof(int)) {
        throw std::runtime_error("unable to read from socket");
    }
    i = ntohl(i);
}

uint32_t RemoteSwitchInfo::processCommand(const string& cmd, Pointer& buf,
                                          uint32_t& outsize)
{
    const static char* SERVER_IP = "127.0.0.1";
    const static int SERVER_PORT = 49152;
    const static int UNKNOWN_ERROR = 1;
    struct sockaddr_in serv_addr{};
    struct sockaddr addr{};
    uint32_t status = UNKNOWN_ERROR;
    uint32_t len;
    uint32_t cmd_len = uint32_t(cmd.length()) + 1;

    int sock_id = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_id  < 0) {
        throw std::runtime_error("failed to open socket");
    }

    ScopedSocket ss(sock_id);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(SERVER_PORT);

    memcpy_s(&addr, sizeof(addr), &serv_addr, sizeof(serv_addr));
    if (connect(sock_id, &addr, sizeof(serv_addr))
        < 0) {
        throw std::runtime_error("failed to connect to server");
    }
    writeInt(sock_id, cmd_len);
    if (write(sock_id, cmd.data(), cmd_len) != cmd_len) {
        throw std::runtime_error("failed to write command");
    }

    readInt(sock_id, len);

    if (len > sizeof(status)) {
        readInt(sock_id, status);
        len -= uint32_t(sizeof(status));
        if (len > 0) {
            Pointer p(new char[len]);
            if(read(sock_id, p.get(), len) != len) {
                throw std::runtime_error("failed to read result");
            }
            else {
                outsize = len;
                buf = std::move(p);
            }
        }
    }
    return status;
}

RemoteSwitchInfo::ScopedSocket::~ScopedSocket() {
    if (0 < m_sock) {
        close(m_sock);
    }
}
