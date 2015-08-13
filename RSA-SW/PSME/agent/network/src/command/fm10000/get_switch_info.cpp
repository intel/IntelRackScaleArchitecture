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
#include "api/netlink/switch_info.hpp"
#include "network_config.hpp"

using std::runtime_error;

using namespace agent_framework::command;
using namespace agent_framework::generic;

namespace agent {
namespace network {
namespace hw {
namespace fm10000 {

using namespace agent_framework;

class GetSwitchInfo : public command::network::GetSwitchInfo {

public:
    GetSwitchInfo() { }

    using agent_framework::command::network::GetSwitchInfo::execute;

    void execute(const Request& request, Response& response) {
#ifdef NL3_FOUND

        try {
            api::netlink::SwitchInfo info(request.get_component());

            if (!info.get_is_enabled()) {
                response.set_status({"Disabled", "Warning"});
                return;
            }

            /* read switch information */
            info.read_switch_info();

            /* return the result */
            response.set_mac_address(info.get_mac_address());
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm10000"), "Cannot get switch information");
            log_debug(GET_LOGGER("fm10000"), error.what());
            throw exception::NotFound();
        }
        response.set_status({"Enabled", "OK"});
        response.set_oem({});
#else
        (void) request;
        response.set_technology("Ethernet");
        response.set_fru_info({ "123fed3029c-b23094-12", "Intel Corporation",
                                "E234", "29ee2220939"});
        response.set_oem({});
#endif /* NL3_FOUND */
    }

    ~GetSwitchInfo();
};

GetSwitchInfo::~GetSwitchInfo() { }

}
}
}
}

static Command::Register<agent::network::hw::fm10000::GetSwitchInfo> g("fm10000");
