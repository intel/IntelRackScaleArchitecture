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

#include "psme/utils/network_interface_info.hpp"
#include "logger_ext.hpp"
#include <safe-string/safe_lib.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string>
#include <unistd.h>
#include <cstring>
#include <exception>

using namespace psme::utils;
using namespace std;

namespace {
    constexpr const int FORMATTED_MAC_ADDRESS_LENGTH = 17;

    string read_ioctl(int fd, unsigned long ioctl_request,
                    const std::string& nic_name, const std::string& msg) {
        struct ifreq ifr;
        struct sockaddr_in sockaddress;
        memset(&ifr, 0, sizeof (ifr));
        ifr.ifr_addr.sa_family = AF_INET;
        strncpy_s(ifr.ifr_name, sizeof (ifr.ifr_name),
                nic_name.c_str(), nic_name.size());

        log_debug(GET_LOGGER("rest"), "Reading interface " << msg
                << ": " << nic_name);

        if (ioctl(fd, ioctl_request, &ifr) < 0) {
            log_warning(GET_LOGGER("rest"), "Cannot read interface " << msg
                    << ": " <<  nic_name);
            throw runtime_error("ioctl failed");
        }

        if (EOK != memcpy_s(&sockaddress, sizeof (sockaddress),
                &ifr.ifr_addr, sizeof (ifr.ifr_addr))) {
            throw std::runtime_error("cannot copy memory");
        }
        char *result = inet_ntoa(sockaddress.sin_addr);

        if (nullptr == result) {
            throw runtime_error("no " + msg);
        }

        std::string address(result);

        log_debug(GET_LOGGER("rest"), msg << " for interface: " <<
                nic_name + " is " + address);
        return address;
    }
}

const NetworkInterfaceInfo::NetworkInterfaceAddress
NetworkInterfaceInfo::get_interface_address() {
    if (m_interface_name.empty()) {
        throw invalid_argument("Interface name is empty");
    }
    NetworkInterfaceInfo::NetworkInterfaceAddress networkInterfaceAddress;

    m_socket_open();

    try {
        auto ip_address = m_get_ip_address();
        networkInterfaceAddress.set_ip_address(ip_address);
    } catch (const exception& ex) {
        log_error(GET_LOGGER("rest"),"Unable to read IP Address " <<
                "for interface: " << m_interface_name << ", " <<
                ex.what());
    }

    try{
        auto netmask = m_get_netmask();
        networkInterfaceAddress.set_netmask(netmask);
    } catch (const exception& ex) {
        log_error(GET_LOGGER("rest"),"Unable to read Netmask Address " <<
                "for interface: " << m_interface_name << ", " <<
                ex.what());
    }

    try{
        auto mac_address = m_get_mac_address();
        networkInterfaceAddress.set_mac_address(mac_address);
    } catch (const exception& ex) {
        log_error(GET_LOGGER("rest"),"Unable to read MAC Address " <<
             "for interface: " << m_interface_name << ", " <<
             ex.what());
    }
    m_socket_close();
    return networkInterfaceAddress;
}


void NetworkInterfaceInfo::m_socket_open() {
    m_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (m_fd < 0) {
        throw runtime_error("Unable to open socket");
    }
}

const string NetworkInterfaceInfo::m_get_ip_address() const {
    return read_ioctl(m_fd, SIOCGIFADDR, m_interface_name, "IP address");
}

const string NetworkInterfaceInfo::m_get_netmask() const {
    return read_ioctl(m_fd, SIOCGIFNETMASK, m_interface_name, "Netmask");
}

const string NetworkInterfaceInfo::m_get_mac_address() const {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    char mac_str[FORMATTED_MAC_ADDRESS_LENGTH + 1];
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), m_interface_name.c_str(), m_interface_name.size());

    int ioctl_exit_code = ioctl(m_fd, SIOCGIFHWADDR, &ifr);

    if (ioctl_exit_code < 0) {
        throw runtime_error("ioctl failed");
    }

    auto sa_data = reinterpret_cast<unsigned char*>(ifr.ifr_hwaddr.sa_data);

    std::snprintf(&mac_str[0], sizeof(mac_str),"%02x:%02x:%02x:%02x:%02x:%02x",
                  static_cast<unsigned int>(sa_data[0]),
                  static_cast<unsigned int>(sa_data[1]),
                  static_cast<unsigned int>(sa_data[2]),
                  static_cast<unsigned int>(sa_data[3]),
                  static_cast<unsigned int>(sa_data[4]),
                  static_cast<unsigned int>(sa_data[5]));

    log_debug(GET_LOGGER("rest"),"MAC address for interface: " <<
            m_interface_name + " is " + string(mac_str));

    return string(mac_str);
}

void NetworkInterfaceInfo::m_socket_close() {
    close(m_fd);
}
