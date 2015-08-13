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
*/

#include "agent-framework/state_machine/state_machine_thread.hpp"
#include "agent-framework/eventing/event_msg.hpp"

using namespace std;
using namespace agent_framework::generic;

StateMachineThread::~StateMachineThread() {
    set_enable(false);
    m_is_running = false;
    m_condition.notify_one();
    m_thread.join();
}

void StateMachineThread::m_module_init_all() {
    /* Init whole Modules passed to thread.  */
    for (auto it = m_modules.begin(); it != m_modules.end(); it++) {
        (*it)->init();
    }
}

void StateMachineThread::m_module_clean_all() {
    /* Clean whole Modules passed to thread.  */
    for (auto it = m_modules.begin(); it != m_modules.end(); it++) {
        (*it)->clean();
    }
}

void StateMachineThread::m_task() {
    ModuleSoftwareStatus *sw_status = nullptr;
    ModuleHardwareStatus *hw_status = nullptr;

    log_info(GET_LOGGER("state-machine"), "Starting State Machine thread...");

    m_module_init_all();

    while(m_is_running) {
        auto now = std::chrono::system_clock::now();

        std::unique_lock<std::mutex> lk(m_mutex);

        // Wait until timeout expired.
        if (m_condition.wait_until(
                    lk,
                    now + chrono::seconds(STATE_MACHINE_INTERVAL_SECONDS)
            ) == std::cv_status::timeout) {
            log_debug(GET_LOGGER("state-machine"), "State Machine iteration.");
            // Iterate through all Modules and try to set next state.
            for (auto it = m_modules.begin(); it != m_modules.end(); it++) {
                auto& state_machine = (*it)->get_state_machine();
                sw_status = (*it)->get_sw_status();
                hw_status = (*it)->get_hw_status();

                // Set next state.
                try {
                    state_machine->set_next_state(hw_status, sw_status);
                    if (state_machine->is_state_changed()) {
                        perform_discovery_if_present(*(it->get()));
                        // Disable agent notifications
                        // notify_all(create_event_msg(it->get()));
                    }
                }
                catch (StateMachineError & e) {
                    log_debug(GET_LOGGER("state-machine"), e.what());
                    log_error(GET_LOGGER("state-machine"), "Cannot set next state.");
                }
            }
        }
    }

    m_module_clean_all();

    // Exiting safty...
    log_debug(GET_LOGGER("state-machine"), "State Machine thread stopped.");
}

void StateMachineThread::perform_discovery_if_present(Module & module) {
    auto& state_machine = module.get_state_machine();
    auto module_state = state_machine->get_curr_state()->get_state();

    if(ModuleState::State::ENABLED == module_state) {
        m_discovery_manager.discover(module);
    }
}


void StateMachineThread::start() {
    m_thread = std::thread(&StateMachineThread::m_task, this);
    m_is_running = true;
}

EventMsg StateMachineThread::create_event_msg(Module* module) {
    const auto& machine = module->get_state_machine();
    EventMsg msg{
        module->get_name(),
        machine->get_curr_state()->get_state(),
        machine->get_curr_transition()->get_transition()
    };

    return msg;
}
