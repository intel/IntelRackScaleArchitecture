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

#include "agent-framework/command/network/get_module_info.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "api/switch_info.hpp"
#include "api/netlink/switch_info.hpp"
#include "network_config.hpp"

using agent_framework::command::exception::NotFound;
using std::runtime_error;

using namespace agent_framework::command;
using namespace agent_framework::generic;

namespace agent {
namespace network {
namespace hw {
namespace fm10000 {

using namespace agent_framework;

/*! Dummy GetModuleInfo implementation */
class GetModuleInfo : public command::network::GetModuleInfo {
public:
    GetModuleInfo() { }

    using agent_framework::command::network::GetModuleInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();
#ifdef NL3_FOUND
        try {
            api::netlink::SwitchInfo info(uuid);

            if (!info.get_is_enabled()) {
                response.set_status({"Disabled", "Warning"});
                return;
            }
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm10000"), "Cannot get module information");
            log_debug(GET_LOGGER("fm10000"), error.what());
            throw exception::NotFound();
        }
#endif /* NL3_FOUND */

        response.set_status({"Enabled", "OK"});
        response.set_switch_count(1);
        response.set_fru_info({ "", "","", ""});
        response.set_oem({});
    }

    ~GetModuleInfo();
};

GetModuleInfo::~GetModuleInfo() { }
}
}
}
}

static Command::Register<agent::network::hw::fm10000::GetModuleInfo> g("fm10000");
