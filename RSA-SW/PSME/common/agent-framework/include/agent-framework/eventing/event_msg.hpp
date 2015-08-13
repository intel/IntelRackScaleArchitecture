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
 * @file event_msg.hpp
 *
 * @brief Eventing Message
 * */

#ifndef AGENT_FRAMEWORK_EVENTING_EVENT_MSG_HPP
#define AGENT_FRAMEWORK_EVENTING_EVENT_MSG_HPP

#include "agent-framework/state_machine/module_state.hpp"
#include "agent-framework/state_machine/state_machine_transition.hpp"

#include <json/json.h>

namespace agent_framework {
/*! Generic namespace */
namespace generic {

    /*!
     * @brief Class represents event message
     */
class EventMsg {
public:

    /*!
     * Event Message constructor
     *
     * @param[in] id Component ID
     * @param[in] state Component State
     * @param[in] transition Component Transition
     * */
    explicit EventMsg(const std::string& id,
                      const ModuleState::State state,
                      const StateMachineTransition::Transition transition):
    m_id{id},
    m_state{state},
    m_transition{transition} {}

    /*!
     * @brief Sets component id
     *
     * @param id component id
     */
    void set_id(const std::string& id) {
        m_id = id;
    }

    /*!
     * @brief Gets component id
     *
     * @return component id
     */
    const std::string& get_id() const {
        return m_id;
    }

    /*!
     * @brief Sets module state
     *
     * @param state module state
     */
    void set_state(const ModuleState::State state) {
        m_state = state;
    }

    /*!
     * @brief Gets module state
     * @return module state
     */
    const ModuleState::State& get_state() const {
        return m_state;
    }

    /*!
     * @brief Sets state machine transition
     *
     * @param transition state machine transition
     */
    void set_transition(const StateMachineTransition::Transition transition) {
        m_transition = transition;
    }

    /*!
     * @brief Gets state machine transition
     *
     * @return state machine transition
     */
    const StateMachineTransition::Transition& get_transition() const {
        return m_transition;
    }

    /*!
     * @brief Convert event message to json format
     *
     * @return message json format
     */
    Json::Value to_json() const;

private:
    std::string m_id;
    ModuleState::State m_state;
    StateMachineTransition::Transition m_transition;
};

}
}

#endif /* AGENT_FRAMEWORK_EVENTING_EVENT_MSG_HPP */
