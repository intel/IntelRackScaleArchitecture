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

#include <cstdio>

#include "../mocks/mock_logger_ext.hpp"
#include "../mocks/mock_module_status.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "agent-framework/state_machine/state_machine.hpp"
#include "agent-framework/state_machine/state_machine_exception.hpp"
#include "agent-framework/state_machine/state_machine_transition.hpp"
#include "agent-framework/status/module_status.hpp"
#include "agent-framework/status/module_hardware_status.hpp"
#include "agent-framework/status/module_software_status.hpp"

using namespace agent_framework::generic;
using namespace agent_framework::testing;
using ::testing::Return;
using ::testing::Throw;


class StateMachineTest : public ::testing::Test {
protected:
    StateMachine state_machine_unknown;
    StateMachine state_machine_up;
    StateMachine state_machine_down;
    StateMachine state_machine_missing;

    MockModuleStatus hw_status;
    MockModuleStatus sw_status;

    logger_cpp::Logger* LOGUSRMOCK = nullptr;

    virtual void SetUp() {

        LOGUSRMOCK = new MockLogger;

        /* State Machine in Unknown state. */
        state_machine_unknown.start();

        /* State Machine in Missing state. */
        state_machine_missing.start();

        ON_CALL(hw_status, read_status())
            .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));
        ON_CALL(sw_status, read_status())
            .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));

        state_machine_missing.set_next_state(&hw_status, &sw_status);

        /* State Machine in Up state. */
        state_machine_up.start();

        ON_CALL(hw_status, read_status())
            .WillByDefault(Return(ModuleStatus::Status::PRESENT));
        ON_CALL(sw_status, read_status())
            .WillByDefault(Return(ModuleStatus::Status::PRESENT));

        state_machine_up.set_next_state(&hw_status, &sw_status);

        /* State Machine in Down state. */
        state_machine_down.start();

        ON_CALL(hw_status, read_status())
            .WillByDefault(Return(ModuleStatus::Status::PRESENT));
        ON_CALL(sw_status, read_status())
            .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));

        state_machine_down.set_next_state(&hw_status, &sw_status);
    }

    virtual void TearDown() {
        state_machine_unknown.stop();
        state_machine_up.stop();
        state_machine_down.stop();
        state_machine_missing.stop();
        delete LOGUSRMOCK;
    }

    virtual ~StateMachineTest();
};

StateMachineTest::~StateMachineTest() {}

/* UNKNOWN */

TEST_F(StateMachineTest, PositiveCreateInUnknown) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_unknown;

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::IDLE);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::UNKNOWN);
}

TEST_F(StateMachineTest, PositiveTransitionFromUnknownToMissing) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_missing;

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::DISCOVERY_MISSING);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::ABSENT);
}

TEST_F(StateMachineTest, PositiveTransitionFromUnknownToUp) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_up;

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::DISCOVERY_UP);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::ENABLED);
}

TEST_F(StateMachineTest, PositiveTransitionFromUnknownToDown) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_down;

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::DISCOVERY_DOWN);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::OFFLINE);
}

TEST_F(StateMachineTest, PositiveTransitiontoMissingWhenStatusUnknown) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_unknown;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::UNKNOWN));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::UNKNOWN));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::DISCOVERY_MISSING);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::ABSENT);
}

/* MISSING */

TEST_F(StateMachineTest, PositiveTransitionFromMissingToUp) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_missing;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::INSERTION);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::ENABLED);
}

TEST_F(StateMachineTest, PositiveTransitionFromMissingToDown) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_missing;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::WENT_DOWN);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::OFFLINE);
}

TEST_F(StateMachineTest, PositiveTransitionStayInMissing) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_missing;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::IDLE);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::ABSENT);
}

/* DOWN */

TEST_F(StateMachineTest, PositiveTransitionFromDownToMissing) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_down;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::EXTRACTION);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::ABSENT);
}

TEST_F(StateMachineTest, PositiveTransitionFromDownToUp) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_down;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::CAME_UP);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::ENABLED);
}

TEST_F(StateMachineTest, PositiveTransitionStayInDown) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_down;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::IDLE);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::OFFLINE);
}

/* UP */

TEST_F(StateMachineTest, PositiveTransitionFromUpToMissing) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_up;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::EXTRACTION);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::ABSENT);
}

TEST_F(StateMachineTest, PositiveTransitionFromUpToDown) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_up;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::WENT_DOWN);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::OFFLINE);
}

TEST_F(StateMachineTest, PositiveTransitionStayInUp) {
    StateMachine *state_machine = nullptr;
    StateMachineTransition *curr_transition = nullptr;
    ModuleState *curr_state = nullptr;

    state_machine = &state_machine_up;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));

    state_machine->set_next_state(&hw_status, &sw_status);

    curr_transition = state_machine->get_curr_transition();
    curr_state = state_machine->get_curr_state();

    ASSERT_EQ(curr_transition->get_transition(),
            StateMachineTransition::Transition::IDLE);
    ASSERT_EQ(curr_state->get_state(), ModuleState::State::ENABLED);
}

TEST_F(StateMachineTest, PositiveStateMachineErrorMessage) {

    const char *msg = "Test message.";
    StateMachineError *error = new StateMachineError(msg);

    ASSERT_STREQ(msg, error->what());

    delete error;
}

/* Negative */

TEST_F(StateMachineTest, NegativeSetNextStateInvalidArguments) {

    StateMachine *state_machine = nullptr;

    state_machine = &state_machine_up;

    EXPECT_THROW(state_machine->set_next_state(nullptr, nullptr),
            std::invalid_argument);
    EXPECT_THROW(state_machine->set_next_state(nullptr, &sw_status),
            std::invalid_argument);
    EXPECT_THROW(state_machine->set_next_state(&hw_status, nullptr),
            std::invalid_argument);
}

TEST_F(StateMachineTest, NegativeCannotGetHardwareStatus) {

    StateMachine *state_machine = nullptr;

    state_machine = &state_machine_up;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Throw(std::exception()));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));

    EXPECT_THROW(state_machine->set_next_state(&hw_status, &sw_status),
            StateMachineError);

    ON_CALL(hw_status, read_status())
        .WillByDefault(Throw(std::exception()));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));

    EXPECT_THROW(state_machine->set_next_state(&hw_status, &sw_status),
            StateMachineError);
}

TEST_F(StateMachineTest, NegativeCannotGetSoftwareStatus) {

    StateMachine *state_machine = nullptr;

    state_machine = &state_machine_up;

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Throw(std::exception()));

    EXPECT_THROW(state_machine->set_next_state(&hw_status, &sw_status),
            StateMachineError);

    ON_CALL(hw_status, read_status())
        .WillByDefault(Return(ModuleStatus::Status::NOT_PRESENT));
    ON_CALL(sw_status, read_status())
        .WillByDefault(Throw(std::exception()));

    EXPECT_THROW(state_machine->set_next_state(&hw_status, &sw_status),
            StateMachineError);
}


