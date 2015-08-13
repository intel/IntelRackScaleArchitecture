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

#include "agent-framework/state_machine/state_machine.hpp"

using namespace std;
using namespace agent_framework::generic;

StateMachine::StateMachine() {
    m_curr_transition->set_transition(StateMachineTransition::Transition::IDLE);
}

StateMachine::~StateMachine() {}

void StateMachine::start() {
    ModuleStateUnknown *module_state_unknown = new ModuleStateUnknown;
    ModuleStateAbsent *module_state_absent = new ModuleStateAbsent;
    ModuleStateEnabled *module_state_enabled = new ModuleStateEnabled;
    ModuleStateOffline *module_state_offline = new ModuleStateOffline;

    // Register all states
    m_add_state(ModuleState::State::UNKNOWN, module_state_unknown);
    m_add_state(ModuleState::State::ABSENT, module_state_absent);
    m_add_state(ModuleState::State::ENABLED, module_state_enabled);
    m_add_state(ModuleState::State::OFFLINE, module_state_offline);

    // Set initial state
    m_curr_state = module_state_unknown;

    log_info(GET_LOGGER("state-machine"), "State Machine initialized.\n");
}

void StateMachine::stop() {
    log_info(GET_LOGGER("state-machine"), "State Machine stopped.");
}

void StateMachine::set_next_state(ModuleHardwareStatus * hw_status,
                                  ModuleSoftwareStatus * sw_status) {

    ModuleState::State next_state = ModuleState::State::UNKNOWN;
    ModuleStatus::Status curr_hw_status = ModuleStatus::Status::UNKNOWN;
    ModuleStatus::Status curr_sw_status = ModuleStatus::Status::UNKNOWN;

    if (nullptr == hw_status || nullptr == sw_status) {
        log_error(GET_LOGGER("state-machine"), "Cannot set next state.");
        log_debug(GET_LOGGER("state-machine"), "Invalid argument passed.");
        throw std::invalid_argument("Incorrect hardware or software status.");
    }

    // Read hardware status.
    try {
        curr_hw_status = hw_status->read_status();
    }
    catch (exception & e) {
        throw StateMachineError(e.what());
    }

    // Read software status.
    try {
        curr_sw_status = sw_status->read_status();
    }
    catch (exception & e) {
        throw StateMachineError(e.what());
    }

    // Determine next state
    if (curr_hw_status == ModuleStatus::Status::PRESENT
        && curr_sw_status == ModuleStatus::Status::PRESENT) {
        next_state = ModuleState::State::ENABLED;
    }
    else if (curr_hw_status == ModuleStatus::Status::PRESENT
        && curr_sw_status == ModuleStatus::Status::NOT_PRESENT) {
        next_state = ModuleState::State::OFFLINE;
    }
    else {
        /* This covers all cases except these listed above. As we
         * cannot move to UNKNOWN from the remaining states, we treat
         * as MISSING following cases:
         *      - Cannot read SW and/or HW status (exceptions thrown by
         *      read_status()).
         *      - HW status is NOT_PRESENT.
         * */
        next_state = ModuleState::State::ABSENT;
    }

    m_curr_transition =
        m_curr_state->enter_state(std::move(m_curr_transition), next_state);
    m_curr_state = m_states[next_state].get();

    log_debug(GET_LOGGER("state-machine"), "Current state: "
            << static_cast<int>(m_curr_state->get_state())
            << ", Current transition: "
            << static_cast<int>(m_curr_transition->get_transition()));
}

void StateMachine::m_add_state(ModuleState::State state_enum, ModuleState * state) {
    if (nullptr == state) {
       throw StateMachineError("State does not exist.");
    }

    // Adds state to map of states.
    m_states[state_enum] = std::move(static_cast<ModuleStateUniquePtr>(state));
}

bool StateMachine::is_state_changed() const {
    return m_curr_transition->get_transition() !=
            StateMachineTransition::Transition::IDLE;
}
