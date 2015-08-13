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
 * @file switch_port_info.cpp
 *
 * @brief Switch port parameters
 * */
#include "switch_port_info.hpp"

using namespace agent::network::api;
using std::runtime_error;

constexpr char SwitchPortInfo::STR_UP[];
constexpr char SwitchPortInfo::STR_DOWN[];
constexpr char SwitchPortInfo::STR_UNKNOWN[];

const SwitchPortInfo::state2string_t SwitchPortInfo::m_state2string = {{
    std::make_pair(SwitchPortInfo::State::UP,      STR_UP),
    std::make_pair(SwitchPortInfo::State::DOWN,    STR_DOWN),
    std::make_pair(SwitchPortInfo::State::UNKNOWN, STR_UNKNOWN),
}};

SwitchPortInfo::SwitchPortInfo(uint8_t switch_id, uint8_t port_index)
    : m_switch_id(switch_id), m_index(port_index) {}

SwitchPortInfo::~SwitchPortInfo() {}

const string& SwitchPortInfo::get_type_str() const {
    for (const auto& value : m_port_type_map) {
        if (value.second == m_type) {
            return value.first;
        }
    }
    throw runtime_error("Can not convert port type");
}

void SwitchPortInfo::set_type(const string& type) {
    auto value = m_port_type_map.find(type);
    if (value != m_port_type_map.end()) {
        m_type = value->second;
        return;
    }
    throw runtime_error("Port type \"" + type + "\" is invalid");
}
