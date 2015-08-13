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
 * */

#include "agent-framework/command/compute/get_blade_info.hpp"

using namespace agent_framework::generic;
using namespace agent_framework::command;

class GetBladeInfo : public agent_framework::command::compute::GetBladeInfo {
public:
    GetBladeInfo() { }

    using agent_framework::command::compute::GetBladeInfo::execute;

    void execute(const Request&, Response& response) {
        response.set_bios_version("A20F21_A0");
        response.set_processor_count(2);
        response.set_dimm_count(16);
        response.set_nic_count(2);
        response.set_controller_count(1);
        response.set_slot(2);
        response.set_power_state("On");
        response.set_boot_override("Disabled");
        response.set_boot_override_target("None");
        response.set_status({"Enabled", "OK"});
        response.set_fru_info({ "123fed3039c-b23394-12",
                                "Intel Corporation",
                                "E323",
                                "29ee2220939"});
        response.set_oem({});
    }

    ~GetBladeInfo();
};

GetBladeInfo::~GetBladeInfo() { }

static Command::Register<GetBladeInfo> g("Intel");
