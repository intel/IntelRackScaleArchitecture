/*!
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
 *
 * @file state_machine.hpp
 * @brief StateMachine
 * */

#ifndef AGENT_FRAMEWORK_STATE_MACHINE_STATE_MACHINE_HPP
#define AGENT_FRAMEWORK_STATE_MACHINE_STATE_MACHINE_HPP

#include <map>
#include <memory>
#include <exception>

#include "agent-framework/logger_ext.hpp"

#include "agent-framework/state_machine/module_state_unknown.hpp"
#include "agent-framework/state_machine/module_state_absent.hpp"
#include "agent-framework/state_machine/module_state_enabled.hpp"
#include "agent-framework/state_machine/module_state_offline.hpp"

#include "agent-framework/state_machine/state_machine_exception.hpp"
#include "agent-framework/state_machine/state_machine_transition.hpp"

#include "agent-framework/status/module_software_status.hpp"
#include "agent-framework/status/module_hardware_status.hpp"

namespace agent_framework {
namespace generic {

/*!
 * @brief Main class of a state machine.
 */
class StateMachine {
    ModuleState * m_curr_state = nullptr;
    StateMachineTransitionUniquePtr
                 m_curr_transition { new StateMachineTransition };

    std::map<ModuleState::State, ModuleStateUniquePtr> m_states{};

    StateMachine & operator=(const StateMachine &) = delete;
    StateMachine(const StateMachine &) = delete;
public:

    /*!
     * @brief Class default constructor.
     */
    StateMachine();

    /*!
     * @brief Class destructor.
     */
    ~StateMachine();

    /*!
     * @brief Starts %StateMachine.
     * @throw StateMachineException.
     */
    void start();

    /*!
     * @brief Stops %StateMachine.
     * @throw StateMachineException.
     */
    void stop();

    /*!
     * @brief Getter to current state of the %StateMachine.
     * @return Pointer to current state.
     */
    inline ModuleState * get_curr_state() const {
        return m_curr_state;
    }

    /*!
     * @brief Getter to current transition of the state machine.
     * @return Pointer to %StateMachine transition.
     */
    inline  StateMachineTransition * get_curr_transition() const {
        return m_curr_transition.get();
    }

    /*!
     * @brief Sets next state.
     */
    void set_next_state(ModuleHardwareStatus * hw_status,
                        ModuleSoftwareStatus * sw_status);

    /*!
     * @brief Check if current state changed
     *
     * @return true if status changed
     */
    bool is_state_changed() const;

private:
    void m_add_state(ModuleState::State state_nr, ModuleState * state);
};

/*! State Machine unique pointer. */
typedef std::unique_ptr<StateMachine> StateMachineUniquePtr;

}
}
#endif /* AGENT_FRAMEWORK_STATE_MACHINE_STATE_MACHINE_HPP */
