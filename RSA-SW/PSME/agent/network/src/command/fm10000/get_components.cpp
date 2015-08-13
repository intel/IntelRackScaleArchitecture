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

#include "agent-framework/module/module.hpp"
#include "agent-framework/command/network/get_components.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

namespace agent {
namespace network {
namespace hw {
namespace fm10000 {

using namespace agent_framework;

/*! Dummy GetComponents implementation */
class GetComponents : public command::network::GetComponents {
public:
    GetComponents() { }

     using agent_framework::command::network::GetComponents::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();
        auto& modules = ModuleManager::get_modules();

        std::vector<Response::Component> vec_sleds{};

        if (uuid.empty()) {
            log_debug(GET_LOGGER("fm10000"), "Getting all componnets.");
            for (auto& module : modules){
                Response::Component component = m_create_compute(module.get());
                vec_sleds.push_back(component);
            }
        }
        else {
            log_debug(GET_LOGGER("fm10000"), "Getting component with uuid=" << uuid);
            auto module = ModuleManager::get_module(uuid);
            auto submodule = ModuleManager::get_submodule(uuid);

            if (nullptr != module) {
                Response::Component component = m_create_compute(module);
                vec_sleds.push_back(component);
            }
            else {
                if (nullptr != submodule) {
                    Response::Component component = m_create_blade(submodule);
                    vec_sleds.push_back(component);
                }
                else {
                    log_warning(GET_LOGGER("fm10000"), "Component uuid=" << uuid << "not found.");
                    throw exception::NotFound();
                }
            }
        }

        response.set_components(vec_sleds);
        log_debug(GET_LOGGER("fm10000"), "Getting components is successfully finished.");
    }

    ~GetComponents();

private:
    Response::Component m_create_compute(Module* module) {
        Response::Component compute;
        compute.set_type(module->get_type());
        compute.set_name(module->get_name());
        std::vector<Response::Component> vec_blades;
        for (auto& submodule : module->get_submodules()){
            vec_blades.push_back(m_create_blade(submodule.get()));
        }
        compute.set_components(vec_blades);
        return compute;
    }

    Response::Component m_create_blade(Submodule* submodule) {
        Response::Component blade;
        blade.set_type(submodule->get_type());
        blade.set_name(submodule->get_name());
        std::vector<Response::Component> vec_blades;
        blade.set_components(vec_blades);
        return blade;
    }
};

GetComponents::~GetComponents() { }

}
}
}
}

static Command::Register<agent::network::hw::fm10000::GetComponents> g("fm10000");
