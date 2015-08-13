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
 * @file module_state.hpp
 * @brief Abstract Module State.
 * */

#ifndef AGENT_FRAMEWORK_STATE_MACHINE_MODULE_STATE_HPP
#define AGENT_FRAMEWORK_STATE_MACHINE_MODULE_STATE_HPP

#include "agent-framework/state_machine/state_machine_transition.hpp"

#include <memory>
#include <array>

/*! Psme namespace */
namespace agent_framework {
/*! Generic namespace */
namespace generic {

/*!
 * @brief Virtual interafce to %StateMachine state.
 */
class ModuleState {
public:
    /*!
     * @enum State
     * @brief Module`s states.
     *
     * @var State ModuleState::UNKNOWN
     * Unknown state.
     *
     * @var State ModuleState::ENABLED
     * Enabled state.
     *
     * @var State ModuleState::OFFLINE
     * Offline state.
     *
     * @var State ModuleState::ABSENT
     * Absent state.
     */
    enum class State {
        UNKNOWN,
        ENABLED,
        OFFLINE,
        ABSENT
    };

    /*! @brief Contains array of states strings. */
    static std::array<const char*, 4> state_names;

private:
    ModuleState(const ModuleState &MS);
    ModuleState & operator=(const ModuleState &MS);
protected:
    /*!
     * @brief Integer value of a particular state.
     */
    State m_state = State::UNKNOWN;
public:
    /*!
     * @brief Class default constructor.
     */
    ModuleState();

    /*!
     * @brief Class constructor.
     * @param[in] state Initial state.
     */
    ModuleState(State state) : m_state(state) {}

    /*!
     * @brief Class destructor.
     */
    virtual ~ModuleState();

    /*!
     * @brief Virtual interface for setting next %ModuleState.
     * @param[in] transition %StateMachine transition object.
     * @param[in] new_state %StateMachine next state.
     */
    virtual StateMachineTransitionUniquePtr enter_state(
            StateMachineTransitionUniquePtr transition,
            ModuleState::State new_state) = 0;

    /*!
     * @brief Getter method for state value.
     * @return Integer value of a partucular state.
     */
    State inline get_state() { return m_state; }

    /**
     * @brief Return state name
     *
     * @param state module state
     *
     * @return state name
     */
    static const char* get_state_name(State state);
};

/*!
 * @brief ModuleState unique pointer.
 */
typedef std::unique_ptr<ModuleState> ModuleStateUniquePtr;

}
}

#endif /* AGENT_FRAMEWORK_STATE_MACHINE_MODULE_STATE_HPP */
