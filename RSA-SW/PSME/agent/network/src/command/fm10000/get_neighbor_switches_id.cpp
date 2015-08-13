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

#include "agent-framework/command/network/get_neighbor_switches_id.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "api/dcrp/remote_switch_info.hpp"

using std::runtime_error;

using namespace agent_framework::command;
using namespace agent_framework::generic;

namespace agent {
namespace network {
namespace hw {
namespace fm10000 {

using namespace agent_framework;

class GetNeighborSwitchesId : public command::network::GetNeighborSwitchesId {

public:
    GetNeighborSwitchesId() { }

    using agent_framework::command::network::GetNeighborSwitchesId::execute;

    void execute(const Request& request, Response& response) {
        const auto& uuid = request.get_component();
        auto switch_id = request.get_switch_id();
        try {
            api::dcrp::RemoteSwitchInfo info(uuid);
            vector<string> switches_id;
            info.get_neighbor_switches_id(switch_id, switches_id);
            for (const auto& neighbor_switch_id : switches_id) {
                response.add_switch_id(neighbor_switch_id);
            }
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("fm10000"), "Cannot get neighbor switches");
            log_debug(GET_LOGGER("fm10000"), error.what());
            throw exception::NotFound();
        }
    }

    ~GetNeighborSwitchesId();
};

GetNeighborSwitchesId::~GetNeighborSwitchesId() { }

}
}
}
}

static Command::Register<agent::network::hw::fm10000::GetNeighborSwitchesId> g("fm10000");
