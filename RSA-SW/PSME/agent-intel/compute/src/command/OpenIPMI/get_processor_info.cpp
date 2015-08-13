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

#include "agent-framework/command/compute/get_processor_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

class GetProcessorInfo : public compute::GetProcessorInfo {
public:
    GetProcessorInfo() { }

    using compute::GetProcessorInfo::execute;

    void execute(const Request&, Response& response) {
        response.set_socket("2");
        response.set_processor_type("CPU");
        response.set_processor_architecture("x86");
        response.set_instruction_set("x64");
        response.set_manufacturer("Intel Corporation");
        response.set_model("E5");

        compute::GetProcessorInfo::Response::CPUID cpuid;
        cpuid.set_vendor_id("GenuineIntel");
        cpuid.set_numeric_id(329442);
        cpuid.set_family(6);
        cpuid.set_model(5);
        cpuid.set_step(2);
        cpuid.set_microcode_info(11);
        response.set_cpuid(cpuid);

        response.set_max_speed_mhz(3700);
        response.set_total_cores(8);
        response.set_enabled_cores(4);
        response.set_total_threads(4);
        response.set_enabled_threads(2);
        response.set_status({"Enabled", "OK"});
    }

    ~GetProcessorInfo();
};

GetProcessorInfo::~GetProcessorInfo() { }

static Command::Register<GetProcessorInfo> g("Intel");
