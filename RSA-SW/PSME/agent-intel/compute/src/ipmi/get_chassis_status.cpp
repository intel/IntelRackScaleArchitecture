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
 * @file get_chassis_status.cpp
 *
 * @brief IPMI Get Chassis Status Command.
 * */
#include "get_chassis_status.hpp"

using namespace agent::compute::ipmi;

request::GetChassisStatus::GetChassisStatus():
    Request(NetFn::CHASSIS, Cmd::GET_CHASSIS_STATUS) {}

request::GetChassisStatus::GetChassisStatus(const GetChassisStatus& orig):
    Request(orig) {}

request::GetChassisStatus::~GetChassisStatus() {}

void request::GetChassisStatus::pack(vector<uint8_t>& data) const {
    (void)data;
}

response::GetChassisStatus::GetChassisStatus():
    Response(NetFn::CHASSIS, Cmd::GET_CHASSIS_STATUS, RESPONSE_SIZE) {}

response::GetChassisStatus::GetChassisStatus(const GetChassisStatus& orig):
    Response(orig) {}
response::GetChassisStatus::~GetChassisStatus() {}

void response::GetChassisStatus::unpack(const vector<uint8_t>& data) {
    if(!is_response_correct(data)) {
        return; // Error response, do not unpack values.
    }
    m_power_on = MASK_SYSTEM_POWER & data[OFFSET_POWER_STATUS];
}

