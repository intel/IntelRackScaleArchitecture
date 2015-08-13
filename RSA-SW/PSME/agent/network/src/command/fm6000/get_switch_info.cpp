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
#include "agent-framework/module/module_manager.hpp"
#include "api/switch_info.hpp"
#include "api/ies/switch_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

namespace agent {
namespace network {
namespace hw {
namespace fm6000 {

using namespace agent_framework;

class GetSwitchInfo : public command::network::GetSwitchInfo {

public:
    GetSwitchInfo() { }

    using agent_framework::command::network::GetSwitchInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();
#ifdef IES_FOUND
        try {
            api::ies::SwitchInfo switch_info(uuid);

            if (!switch_info.get_is_enabled()) {
                response.set_status({"Absent", "Warning"});
                return;
            }

            switch_info.read_switch_info();
            response.set_mac_address(switch_info.get_mac_address());
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm6000"), "Cannot get switch information");
            log_debug(GET_LOGGER("fm6000"), error.what());
            throw exception::NotFound();
        }
        response.set_status({"Enabled", "OK"});
        response.set_technology("Ethernet");
        response.set_oem({});
#else
        response.set_status({"Enabled", "OK"});
        response.set_technology("Ethernet");
        response.set_mac_address("3F:2A:00:10:23:B8");
        response.set_fru_info({ "123fed3029c-b23094-12", "Intel Corporation",
                                "E234", "29ee2220939"});
        response.set_oem({});
#endif /* IES_FOUND */
    }

    ~GetSwitchInfo();
};

GetSwitchInfo::~GetSwitchInfo() { }
}
}
}
}

static Command::Register<agent::network::hw::fm6000::GetSwitchInfo> g("fm6000");
