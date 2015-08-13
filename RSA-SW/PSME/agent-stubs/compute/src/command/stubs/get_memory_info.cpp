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

#include "agent-framework/command/compute/get_memory_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

class GetMemoryInfo : public compute::GetMemoryInfo {
public:
    GetMemoryInfo() { }

    using agent_framework::command::compute::GetMemoryInfo::execute;

    void execute(const Request&, Response& response) {
        response.set_socket("0x2");
        response.set_type("DDR-3 RAM");
        response.set_size_gb(16);
        response.set_speed_mhz(1600);
        response.set_voltage_volt(1.35);
        response.set_data_width_bits(8);
        response.set_total_width_bits(8);
        response.set_form_factor("DIMM");
        response.set_status({"Enabled", "OK"});
        response.set_fru_info({ "123fed3029c-b23394-12",
                                "Intel Corporation",
                                "E323",
                                "29ee2220939"});
    }

    ~GetMemoryInfo();
};

GetMemoryInfo::~GetMemoryInfo() { }

static Command::Register<GetMemoryInfo> g("Stubs");
