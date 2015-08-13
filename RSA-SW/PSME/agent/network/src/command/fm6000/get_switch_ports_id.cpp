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

#include "agent-framework/command/network/get_switch_ports_id.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "api/ies/switch_info.hpp"

using std::runtime_error;

using namespace agent_framework::command;
using namespace agent_framework::generic;

namespace agent {
namespace network {
namespace hw {
namespace fm6000 {

using namespace agent_framework;

class GetSwitchPortsId : public command::network::GetSwitchPortsId {

public:
    GetSwitchPortsId() { }

    using agent_framework::command::network::GetSwitchPortsId::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();

#ifdef IES_FOUND
        try {
            api::ies::SwitchInfo info(uuid);
            info.read_switch_port_list();
            const auto& ports = info.get_port_list();
            for (const auto& port_id : ports) {
                response.add_port_id(port_id);
            }
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm6000"), "Cannot get switch ports id");
            log_debug(GET_LOGGER("fm6000"), error.what());
            throw exception::NotFound();
        }
#else
        (void)response;
#endif /* IES_FOUND */
    }

    ~GetSwitchPortsId();
};

GetSwitchPortsId::~GetSwitchPortsId() { }

}
}
}
}

static Command::Register<agent::network::hw::fm6000::GetSwitchPortsId> g("fm6000");
