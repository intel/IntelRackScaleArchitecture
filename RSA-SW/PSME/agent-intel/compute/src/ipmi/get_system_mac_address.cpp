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
 * @file get_system_mac_address.cpp
 *
 * @brief Implementation of GetSystemMacAddress Request/Response.
 * */
#include "get_system_mac_address.hpp"

using namespace agent::compute::ipmi;

request::GetSystemMacAddress::GetSystemMacAddress(): Request(NetFn::APP, Cmd::GET_SYSTEM_GUID_INFO) {}
request::GetSystemMacAddress::GetSystemMacAddress(const GetSystemMacAddress& orig): Request(orig) {}
request::GetSystemMacAddress::~GetSystemMacAddress() {}

void request::GetSystemMacAddress::pack(vector<uint8_t>& data) const {
    uint8_t interface_id_high = uint8_t((m_interface_id & 0xFF00) >> 8);
    uint8_t interface_id_low  = uint8_t(m_interface_id & 0x00FF);
    data.push_back(interface_id_high);
    data.push_back(interface_id_low);
}

response::GetSystemMacAddress::GetSystemMacAddress(): Response(NetFn::APP, Cmd::GET_SYSTEM_GUID_INFO, RESPONSE_SIZE) {}
response::GetSystemMacAddress::GetSystemMacAddress(const GetSystemMacAddress& orig): Response(orig) {}
response::GetSystemMacAddress::~GetSystemMacAddress() {}

void response::GetSystemMacAddress::unpack(const vector<uint8_t>& data) {
    char mac_str[MAC_STR_SIZE];
    if(!is_response_correct(data)) {
        return; // received only completion code, do not unpack.
    }
    std::snprintf(&mac_str[0], sizeof(mac_str),"%02x:%02x:%02x:%02x:%02x:%02x",
            data[MAC_ADDRESS_OFFSET + 0], data[MAC_ADDRESS_OFFSET + 1], data[MAC_ADDRESS_OFFSET + 2],
            data[MAC_ADDRESS_OFFSET + 3], data[MAC_ADDRESS_OFFSET + 4], data[MAC_ADDRESS_OFFSET + 5]);

    m_system_mac_address = string(mac_str);
    log_debug(GET_LOGGER("agent"), "System MAC address: " << m_system_mac_address);
}

