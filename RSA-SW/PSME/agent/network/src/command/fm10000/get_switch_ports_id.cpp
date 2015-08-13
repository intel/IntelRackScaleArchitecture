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

class GetSwitchPortsId : public command::network::GetSwitchPortsId {

public:
    GetSwitchPortsId() { }

    using agent_framework::command::network::GetSwitchPortsId::execute;

    void execute(const Request& request, Response& response) {
#ifdef NL3_FOUND
        try {
            api::netlink::SwitchInfo info(request.get_component());
            info.read_switch_port_list();
            const auto& ports = info.get_port_list();
            for (const auto& port_id : ports) {
                response.add_port_id(port_id);
            }
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm10000"), "Cannot get switch ports id");
            log_debug(GET_LOGGER("fm10000"), error.what());
            throw exception::NotFound();
        }
#else
        (void) request;
        (void) response;
#endif
    }

    ~GetSwitchPortsId();
};

GetSwitchPortsId::~GetSwitchPortsId() { }

}
}
}
}

static Command::Register<agent::network::hw::fm10000::GetSwitchPortsId> g("fm10000");
