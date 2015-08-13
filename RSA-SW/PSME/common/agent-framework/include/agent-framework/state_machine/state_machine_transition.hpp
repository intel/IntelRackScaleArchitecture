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
 * @file state_machine_transition.hpp
 * @brief %StateMachine abstract transition
 */

#ifndef AGENT_FRAMEWORK_STATE_MACHINE_STATE_MACHINE_TRANSITION_HPP
#define AGENT_FRAMEWORK_STATE_MACHINE_STATE_MACHINE_TRANSITION_HPP

#include <memory>
#include <cstdio>
#include <array>

/*! Psme namespace */
namespace agent_framework {
/*! Generic namespace */
namespace generic {

/*!
 * @brief Virtual interafce to %StateMachine transition.
 */
class StateMachineTransition {
public:
    /*!
     * @enum Transition
     * @brief Module`s transitions.
     *
     * @var Transition StateMachineTransition::IDLE
     * State is not changed.
     *
     * @var Transition StateMachineTransition::EXTRACTION
     * %StateMachine went to MISSING from UP or DOWN state.
     *
     * @var Transition StateMachineTransition::INSERTION
     * %StateMachine went to UP from MISSING state.
     *
     * @var Transition StateMachineTransition::WENT_DOWN
     * %StateMachine went to DOWN from UP or MISSING state.
     *
     * @var Transition StateMachineTransition::CAME_UP
     * %StateMachine went to UP from DOWN state.
     *
     * @var Transition StateMachineTransition::DISCOVERY_UP
     * %StateMachine went to UP from UNKNOWN state.
     *
     * @var Transition StateMachineTransition::DISCOVERY_DOWN
     * %StateMachine went to DOWN from UNKNOWN state.
     *
     * @var Transition StateMachineTransition::DISCOVERY_MISSING
     * %StateMachine went to MISSING from UNKNOWN state.
     */
    enum class Transition {
        IDLE,
        EXTRACTION,
        INSERTION,
        WENT_DOWN,
        CAME_UP,
        DISCOVERY_UP,
        DISCOVERY_DOWN,
        DISCOVERY_MISSING
    };

    /**
     * @brief Transition names array
     */
    static std::array<const char*, 8> transition_names;

private:
    StateMachineTransition(const StateMachineTransition &MT);
    StateMachineTransition & operator=(const StateMachineTransition &MT);
    Transition m_transition = Transition::IDLE;
public:
    /*!
     * @brief Default class constructor.
     */
    StateMachineTransition();

    /*!
     * @brief Class constructor.
     * @param[in] transition Initial transition.
     */
    StateMachineTransition(Transition transition) : m_transition(transition) {}

    /*!
     * @brief Class destructor.
     */
    ~StateMachineTransition();

    /*!
     * @brief Getter method for transition value.
     * @return Integer value of a partucular transition.
     */
    Transition inline get_transition() const { return m_transition; }

    /*!
     * @brief Setter method for transition value.
     * @param transition Transition value to set.
     */
    void inline set_transition(const Transition transition) {
        m_transition = transition;
    }

    /**
     * @brief Gets transition name
     *
     * @param transition state machine transition
     *
     * @return transition name
     */
    static const char* get_transition_name(const Transition transition);
};

/*!
 * @brief State Machine transition unique pointer.
 */
typedef std::unique_ptr<StateMachineTransition> StateMachineTransitionUniquePtr;

}
}
#endif /* AGENT_FRAMEWORK_STATE_MACHINE_STATE_MACHINE_TRANSITION_HPP */


