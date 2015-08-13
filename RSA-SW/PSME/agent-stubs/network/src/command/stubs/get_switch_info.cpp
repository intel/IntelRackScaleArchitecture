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

#include "agent-framework/command/network/get_switch_info.hpp"

using namespace agent_framework;
using namespace agent_framework::command;

class GetSwitchInfo : public command::network::GetSwitchInfo {
public:
    GetSwitchInfo() { }

    using agent_framework::command::network::GetSwitchInfo::execute;

    void execute(const Request&, Response& response) {
        response.set_status({"Enabled", "OK"});
        response.set_mac_address("AA:BB:CC:DD:EE:FF");
        response.set_technology("Ethernet");
        response.set_fru_info({ "123fed3029c-b23094-12", "Intel Corporation",
                                "E234", "29ee2220939"});
        response.set_oem({});
    }

    ~GetSwitchInfo();
};

GetSwitchInfo::~GetSwitchInfo() { }

static Command::Register<GetSwitchInfo> g("Stubs");
