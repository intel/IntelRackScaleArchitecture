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
#include "api/ies/switch_info.hpp"

using namespace agent_framework::generic;
using namespace agent_framework::command;

using agent_framework::command::exception::NotFound;

namespace agent {
namespace network {
namespace hw {
namespace fm6000 {

using namespace agent_framework;

/*! Dummy GetModuleInfo implementation */
class GetModuleInfo : public command::network::GetModuleInfo {
public:
    GetModuleInfo() { }

    using agent_framework::command::network::GetModuleInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();
#ifdef IES_FOUND
        try {
            api::ies::SwitchInfo switch_info(uuid);

            if (!switch_info.get_is_enabled()) {
                response.set_status({"Absent", "Warning"});
                return;
            }
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm6000"), "Cannot get module information");
            log_debug(GET_LOGGER("fm6000"), error.what());
            throw exception::NotFound();
        }
#endif /* IES_FOUND */

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

static Command::Register<agent::network::hw::fm6000::GetModuleInfo> g("fm6000");
