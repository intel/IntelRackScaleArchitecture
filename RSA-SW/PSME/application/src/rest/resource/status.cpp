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
 * */

#include "psme/rest/resource/status.hpp"
#include "psme/rest/utils/enum_chars.hpp"

using namespace psme::rest::resource;

namespace psme {
namespace rest {
namespace utils {

template<>
void get_enum_chars_array<StateType>(EnumChars<StateType>*& ptr, size_t& size) {
    static EnumChars<StateType> g_state_type_chars[] = {
        { StateType::ENABLED, "Enabled"},
        { StateType::DISABLED, "Disabled"},
        { StateType::OFFLINE, "Offline"},
        { StateType::IN_TEST, "InTest"},
        { StateType::STARTING, "Starting"},
        { StateType::ABSENT, "Absent"}
    };
    ptr = g_state_type_chars;
    size = array_size(g_state_type_chars);
}

template<>
void get_enum_chars_array<HealthType>(EnumChars<HealthType>*& ptr, size_t& size) {
    static EnumChars<HealthType> g_health_type_chars[] = {
        { HealthType::OK, "OK"},
        { HealthType::WARNING, "Warning"},
        { HealthType::CRITICAL, "Critical"}
    };
    ptr = g_health_type_chars;
    size = array_size(g_health_type_chars);
}

template<>
void get_enum_chars_array<EnumStatus>(EnumChars<EnumStatus>*& ptr, size_t& size) {
    static EnumChars<EnumStatus> g_health_type_chars[] = {
        { EnumStatus::ENUMERATED, "Enumerated"},
        { EnumStatus::NOT_ENUMERATED, "NotEnumerated"}
    };
    ptr = g_health_type_chars;
    size = array_size(g_health_type_chars);
}

}
}
}

constexpr char Status::STATUS[];
constexpr char Status::STATE[];
constexpr char Status::HEALTH[];

using namespace psme::rest::utils;

Status::Status() : m_state(StateType::ABSENT), m_health(HealthType::WARNING) { }
Status::Status(StateType state, HealthType health)
    : m_state(state), m_health(health) { }
Status::~Status() { }

json::Value
Status::as_json() const {
    json::Value json = json::Value::Type::OBJECT;
    json[STATE] = to_string(m_state);
    if (StateType::ABSENT != m_state) {
        json[HEALTH] = to_string(m_health);
    }
    return json;
}

constexpr char StatusWithRollup::HEALTH_ROLLUP[];

StatusWithRollup::StatusWithRollup()
    : Status(), m_health_rollup(HealthType::WARNING) { }
StatusWithRollup::StatusWithRollup(
    StateType state, HealthType health, HealthType health_rollup)
    : Status(state, health), m_health_rollup(health_rollup) { }

json::Value
StatusWithRollup::as_json() const {
    auto json = Status::as_json();
    if (StateType::ABSENT != get_state()) {
        json[HEALTH_ROLLUP] = to_string(m_health_rollup);
    }
    return json;
}
