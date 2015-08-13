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

#include "agent-framework/state_machine/module_state_enabled.hpp"

using namespace agent_framework::generic;

StateMachineTransitionUniquePtr ModuleStateEnabled::enter_state(
        StateMachineTransitionUniquePtr transition,
        ModuleState::State next_state) {

    switch (next_state) {
    case State::UNKNOWN:
        break;
    case State::ENABLED:
        transition->set_transition(StateMachineTransition::Transition::IDLE);
        break;
    case State::OFFLINE:
        transition->set_transition(StateMachineTransition::Transition::WENT_DOWN);
        break;
    case State::ABSENT:
        transition->set_transition(StateMachineTransition::Transition::EXTRACTION);
        break;
    default:
        break;
    }

    return std::move(transition);
}

