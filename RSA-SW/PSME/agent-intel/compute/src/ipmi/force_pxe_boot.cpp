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
 * @file force_pxe_boot.cpp
 *
 * @brief Force Pxe Boot Command Implementation.
 * */
#include "force_pxe_boot.hpp"

using namespace agent::compute::ipmi;

constexpr std::uint8_t request::ForcePxeBoot::OPTION_BOOT_FLAGS_VALID;
constexpr std::uint8_t request::ForcePxeBoot::OPTION_BOOT_DEVICE_FORCE_PXE;
constexpr std::uint8_t request::ForcePxeBoot::OPTION_SYSTEM_DEFAULT;
constexpr std::uint8_t request::ForcePxeBoot::OPTION_BIOS_DEFAULT;
constexpr std::uint8_t request::ForcePxeBoot::OPTION_BOOT_DEVICE_INSTANCE_FORCE_PXE;

request::ForcePxeBoot::ForcePxeBoot():
    request::SetSystemBootOptions() {}

request::ForcePxeBoot::ForcePxeBoot(const ForcePxeBoot& orig):
    request::SetSystemBootOptions(orig) {}

request::ForcePxeBoot::~ForcePxeBoot() {}

void request::ForcePxeBoot::pack(vector<uint8_t>& data) const {
    using std::uint8_t;
    uint8_t param_validity = uint8_t(ParameterValidity::VALID);
    uint8_t param_selector = uint8_t(ParameterSelector::BOOT_FLAGS);

    uint8_t param_options = uint8_t(
                                param_validity << 7 |
                                param_selector << 0
                            );

    data.push_back(param_options);

    // 5 bytes of the options for BOOT FLAGS. (Set Boot Priority Order).
    data.push_back(OPTION_BOOT_FLAGS_VALID);
    data.push_back(OPTION_BOOT_DEVICE_FORCE_PXE);
    data.push_back(OPTION_SYSTEM_DEFAULT);
    data.push_back(OPTION_BIOS_DEFAULT);
    data.push_back(OPTION_BOOT_DEVICE_INSTANCE_FORCE_PXE);
}

response::ForcePxeBoot::ForcePxeBoot():
    response::SetSystemBootOptions() {}

response::ForcePxeBoot::ForcePxeBoot(const ForcePxeBoot& orig):
    response::SetSystemBootOptions(orig) {}

response::ForcePxeBoot::~ForcePxeBoot() {}

void response::ForcePxeBoot::unpack(const vector<uint8_t>& data) {
    if(!is_response_correct(data)) {
        return;
    }
}
