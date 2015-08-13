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

#include "agent-framework/eventing/event_queue.hpp"
#include "agent-framework/eventing/event_msg.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::generic;
using ::testing::Return;
using ::testing::Throw;

class EventQueueTest : public ::testing::Test {
protected:

    std::unique_ptr<EventQueue<EventMsg>> m_queue{};

    virtual void SetUp() {
        m_queue.reset(new EventQueue<EventMsg>);
    }

    virtual void TearDown() {
        m_queue.reset(nullptr);
    }

    virtual ~EventQueueTest();
};

EventQueueTest::~EventQueueTest() {}

/* Positive. */

TEST_F(EventQueueTest, PositiveEmptyQueue) {
    std::shared_ptr<EventMsg> msg = m_queue->try_pop();
    ASSERT_EQ(msg.get(), nullptr);
}

TEST_F(EventQueueTest, PositiveQueuePushPop) {
    EventMsg in_msg(
        "TestEventId",
        ModuleState::State::UNKNOWN,
        StateMachineTransition::Transition::IDLE);

    m_queue->push_back(in_msg);

    std::shared_ptr<EventMsg> out_msg = m_queue->try_pop();

    ASSERT_NE(out_msg.get(), nullptr);
    ASSERT_EQ(out_msg->get_id(), "TestEventId");
}
