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

#include "agent-framework/command/network/add_port_vlan.hpp"
#include "json/add_port_vlan.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class AddPortVlan : public network::AddPortVlan {
private:
    std::string m_component{};
    std::string m_port_identifier{};
    std::uint32_t m_vlan_id{};
    bool m_tagged{};
public:
    AddPortVlan(
        std::string component,
        std::string port_identifier,
        std::uint32_t vlan_id,
        bool tagged) {
        m_component = component;
        m_port_identifier = port_identifier;
        m_vlan_id = vlan_id;
        m_tagged = tagged;
        }

    using network::AddPortVlan::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();
        auto port_identifier = request.get_port_identifier();
        auto vlan_id = request.get_vlan_id();
        auto tagged = request.get_tagged();

        if (component != m_component
            || port_identifier != m_port_identifier
            || vlan_id != m_vlan_id
            || tagged != m_tagged) {
            throw exception::NotFound();
        }

        response.set_vlan_identifier("Test Vlan Id");
        response.set_oem(agent_framework::generic::OEMData());
    }

    virtual ~AddPortVlan();
};

AddPortVlan::~AddPortVlan() { }

class AddPortVlanTest : public ::testing::Test {
protected:
    static constexpr char COMPONENT[] = "component";
    static constexpr char PORT_ID[] = "portIdentifier";
    static constexpr char VLAN_ID[] = "vlanId";
    static constexpr char TAGGED[] = "tagged";
    static constexpr char VLAN_IDENTIFIER[] = "vlanIdentifier";
    static constexpr char OEM[] = "oem";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";
    static constexpr char TEST_PORT_ID[] = "TestPortId";
    static constexpr const uint TEST_VLAN_ID = 1;
    static constexpr const bool TEST_TAGGED = true;

    virtual ~AddPortVlanTest();
};

constexpr char AddPortVlanTest::COMPONENT[];
constexpr char AddPortVlanTest::PORT_ID[];
constexpr char AddPortVlanTest::VLAN_ID[];
constexpr char AddPortVlanTest::TAGGED[];
constexpr char AddPortVlanTest::VLAN_IDENTIFIER[];
constexpr char AddPortVlanTest::OEM[];
constexpr char AddPortVlanTest::TEST_UUID[];
constexpr char AddPortVlanTest::TEST_PORT_ID[];

AddPortVlanTest::~AddPortVlanTest() { }

TEST_F(AddPortVlanTest, PositiveExecute) {
    network::json::AddPortVlan command_json;
    AddPortVlan* command = new AddPortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID, TEST_TAGGED);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = TEST_VLAN_ID;
    params[TAGGED] = TEST_TAGGED;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[VLAN_IDENTIFIER].isString());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_EQ(result[VLAN_IDENTIFIER].asString(), "Test Vlan Id");
}

TEST_F(AddPortVlanTest, NegativeComponentNotFound) {
    network::json::AddPortVlan command_json;
    AddPortVlan* command = new AddPortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID, TEST_TAGGED);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = TEST_VLAN_ID;
    params[TAGGED] = TEST_TAGGED;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(AddPortVlanTest, NegativeInvalidUUIDFormat) {
    network::json::AddPortVlan command_json;
    AddPortVlan* command = new AddPortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID, TEST_TAGGED);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = TEST_VLAN_ID;
    params[TAGGED] = TEST_TAGGED;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(AddPortVlanTest, NegativePortNotFound) {
    network::json::AddPortVlan command_json;
    AddPortVlan* command = new AddPortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID, TEST_TAGGED);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_ID] = "OtherPortId";
    params[VLAN_ID] = TEST_VLAN_ID;
    params[TAGGED] = TEST_TAGGED;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(AddPortVlanTest, NegativeVlanNotFound) {
    network::json::AddPortVlan command_json;
    AddPortVlan* command = new AddPortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID, TEST_TAGGED);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = 2;
    params[TAGGED] = TEST_TAGGED;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(AddPortVlanTest, NegativeTaggedNotFound) {
    network::json::AddPortVlan command_json;
    AddPortVlan* command = new AddPortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID, TEST_TAGGED);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = TEST_VLAN_ID;
    params[TAGGED] = false;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(AddPortVlanTest, NegativeVlanCommandError) {
    network::json::AddPortVlan command_json;
    AddPortVlan* command = new AddPortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID, TEST_TAGGED);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = "OtherVlanId";
    params[TAGGED] = TEST_TAGGED;

    EXPECT_ANY_THROW(command_json.method(params, result));
}
