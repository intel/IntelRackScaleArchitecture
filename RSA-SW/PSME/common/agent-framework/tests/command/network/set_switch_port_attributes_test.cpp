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

#include "agent-framework/command/network/set_switch_port_attributes.hpp"
#include "json/set_switch_port_attributes.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class TestSwitchPortAttributes {
public:
    TestSwitchPortAttributes() {}
    void set_component(const std::string& val) {
        m_component = val;
    }

    void set_port_identifier(const std::string& val) {
        m_port_identifier = val;
    }

    void set_link_speed_gbps(uint32_t& val) {
        m_link_speed_gbps = val;
    }

    void set_administrative_state(const std::string& val) {
        m_administrative_state = val;
    }

    void set_frame_size(uint32_t& val) {
        m_frame_size = val;
    }

    void set_auto_sense(const bool& val) {
        m_auto_sense = val;
    }

    const std::string& get_component() const {
        return m_component;
    }

    const std::string& get_port_identifier() const {
        return m_port_identifier;
    }

    uint32_t get_link_speed_gbps() const {
        return m_link_speed_gbps;
    }

    const std::string& get_administrative_state() const {
        return m_administrative_state;
    }

    uint32_t get_frame_size() const {
        return m_frame_size;
    }

    bool get_auto_sense() const {
        return m_auto_sense;
    }
    virtual ~TestSwitchPortAttributes();
private:
    std::string m_component;
    std::string m_port_identifier;
    uint32_t m_link_speed_gbps;
    std::string m_administrative_state;
    uint32_t m_frame_size;
    bool m_auto_sense;
};

TestSwitchPortAttributes::~TestSwitchPortAttributes() { }

class SetSwitchPortAttributes : public network::SetSwitchPortAttributes {
private:
    std::string m_component{};
    TestSwitchPortAttributes* m_switch_port_attributes;
public:
    SetSwitchPortAttributes(
        const std::string& component,
        TestSwitchPortAttributes* switch_port_attributes) {
        m_component = component;
        m_switch_port_attributes = switch_port_attributes;
        }

    using network::SetSwitchPortAttributes::execute;

    void execute(const Request& request, Response&) {
        auto component = request.get_component();
        auto port_identifier = request.get_port_identifier();
        auto link_speed_gbps = request.get_link_speed_gbps();
        auto administrative_state = request.get_administrative_state();
        auto frame_size = request.get_frame_size();
        auto auto_sense = request.get_auto_sense();

        if (component != m_component ) {
                throw exception::NotFound();
        }

        m_switch_port_attributes->set_component(component);
        m_switch_port_attributes->set_port_identifier(port_identifier);
        m_switch_port_attributes->set_link_speed_gbps(link_speed_gbps);
        m_switch_port_attributes->set_administrative_state(administrative_state);
        m_switch_port_attributes->set_frame_size(frame_size);
        m_switch_port_attributes->set_auto_sense(auto_sense);
    }

    virtual ~SetSwitchPortAttributes();
};

SetSwitchPortAttributes::~SetSwitchPortAttributes() { }

class SetSwitchPortAttributesTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char PORT_IDENTIFIER[] = "portIdentifier";
    static constexpr char LINK_SPEED_GBPS[] = "linkSpeedGbps";
    static constexpr char ADMINISTARTIVE_STATE[] = "administrativeState";
    static constexpr char FRAME_SIZE[] = "frameSize";
    static constexpr char AUTOSENSE[] = "autoSense";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";

    virtual ~SetSwitchPortAttributesTest();
};

constexpr char SetSwitchPortAttributesTest::COMPONENT[];
constexpr char SetSwitchPortAttributesTest::PORT_IDENTIFIER[];
constexpr char SetSwitchPortAttributesTest::LINK_SPEED_GBPS[];
constexpr char SetSwitchPortAttributesTest::ADMINISTARTIVE_STATE[];
constexpr char SetSwitchPortAttributesTest::FRAME_SIZE[];
constexpr char SetSwitchPortAttributesTest::AUTOSENSE[];
constexpr char SetSwitchPortAttributesTest::TEST_UUID[];

SetSwitchPortAttributesTest::~SetSwitchPortAttributesTest() { }

TEST_F(SetSwitchPortAttributesTest, PositiveExecute) {
    network::json::SetSwitchPortAttributes command_json;
    TestSwitchPortAttributes test_switch_port_attributes;
    SetSwitchPortAttributes *command = new SetSwitchPortAttributes(TEST_UUID, &test_switch_port_attributes);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;
    params[COMPONENT] = TEST_UUID;
    params[PORT_IDENTIFIER] = "TestPortIdentifier";
    params[LINK_SPEED_GBPS] = 1;
    params[ADMINISTARTIVE_STATE] = "TestAdministrativeState";
    params[FRAME_SIZE] = 1;
    params[AUTOSENSE] = true;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_EQ(test_switch_port_attributes.get_port_identifier(), "TestPortIdentifier");
    ASSERT_EQ(test_switch_port_attributes.get_link_speed_gbps(), 1);
    ASSERT_EQ(test_switch_port_attributes.get_component(), TEST_UUID);
    ASSERT_EQ(test_switch_port_attributes.get_administrative_state(), "TestAdministrativeState");
    ASSERT_EQ(test_switch_port_attributes.get_frame_size(), 1);
    ASSERT_EQ(test_switch_port_attributes.get_auto_sense(), true);
}

TEST_F(SetSwitchPortAttributesTest, NegativeModuleNotFound) {
    network::json::SetSwitchPortAttributes command_json;
    TestSwitchPortAttributes test_switch_port_attributes;
    SetSwitchPortAttributes *command = new SetSwitchPortAttributes(TEST_UUID, &test_switch_port_attributes);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;
    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";
    params[PORT_IDENTIFIER] = "TestPortIdentifier";
    params[LINK_SPEED_GBPS] = 1;
    params[ADMINISTARTIVE_STATE] = "TestAdministrativeState";
    params[FRAME_SIZE] = 1;
    params[AUTOSENSE] = true;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(SetSwitchPortAttributesTest, NegativeInvalidUUIDFormat) {
    network::json::SetSwitchPortAttributes command_json;
    TestSwitchPortAttributes test_switch_port_attributes;
    SetSwitchPortAttributes *command = new SetSwitchPortAttributes(TEST_UUID, &test_switch_port_attributes);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;
    params[COMPONENT] = "TestUUID";
    params[PORT_IDENTIFIER] = "TestPortIdentifier";
    params[LINK_SPEED_GBPS] = 1;
    params[ADMINISTARTIVE_STATE] = "TestAdministrativeState";
    params[FRAME_SIZE] = 1;
    params[AUTOSENSE] = true;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
