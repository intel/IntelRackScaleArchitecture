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
 * @file chassis_control_command.cpp
 *
 * @brief IPMI Chassis Control Command
 * */

#include "chassis_control_command.hpp"

using namespace agent::compute::ipmi;

/*
 * Mapping of PSME API and GAMI possible Blade power state to power levels
 * defined by IPMI specification.
 */
using PowerState = request::ChassisControlCommand::PowerState;
const std::map<std::string, PowerState>
    request::ChassisControlCommand::m_map = {
        {"ForceOff",     PowerState::POWER_DOWN},
        {"On",           PowerState::POWER_UP},
        {"SoftShutdown", PowerState::ACPI_SOFT_SHUTDOWN}
    };

request::ChassisControlCommand::ChassisControlCommand()
    :Request(NetFn::CHASSIS, Cmd::CHASSIS_CONTROLL_COMMAND) {}
request::ChassisControlCommand::~ChassisControlCommand() {}

void
request::ChassisControlCommand::pack(vector<uint8_t>& data) const {
    data.push_back(uint8_t(m_power_state));
}

void
request::ChassisControlCommand::set_power_state(PowerState status) {
    m_power_state = status;
}

void
request::ChassisControlCommand::set_power_state(const std::string& power_state) {

    if (!m_map.count(power_state)) {
        throw std::runtime_error("Unsupported Power State: " + power_state);
    }
    set_power_state(m_map.at(power_state));
}

response::ChassisControlCommand::ChassisControlCommand()
    : Response(NetFn::CHASSIS,Cmd::CHASSIS_CONTROLL_COMMAND, RESPONSE_SIZE) {}
response::ChassisControlCommand::~ChassisControlCommand() {}

void
response::ChassisControlCommand::unpack(const vector<uint8_t>& data) {
    if(!is_response_correct(data)) {
        return;
    }
}
