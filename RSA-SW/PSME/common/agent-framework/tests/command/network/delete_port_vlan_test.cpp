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

#include "agent-framework/command/network/delete_port_vlan.hpp"
#include "json/delete_port_vlan.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class DeletePortVlan : public network::DeletePortVlan {
private:
    std::string m_component{};
    std::string m_port_identifier{};
    std::string m_vlan_id{};
public:
    DeletePortVlan(
        std::string component,
        std::string port_identifier,
        std::string vlan_id) {
        m_component = component;
        m_port_identifier = port_identifier;
        m_vlan_id = vlan_id;
        }

    using network::DeletePortVlan::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();
        auto port_identifier = request.get_port_identifier();
        auto vlan_id = request.get_vlan_identifier();

        if (component != m_component
            || port_identifier != m_port_identifier
            || vlan_id != m_vlan_id) {
            throw exception::NotFound();
        }

        response.set_oem(agent_framework::generic::OEMData());
    }

    virtual ~DeletePortVlan();
};

DeletePortVlan::~DeletePortVlan() { }

class DeletePortVlanTest : public ::testing::Test {
protected:
    static constexpr char COMPONENT[] = "component";
    static constexpr char PORT_ID[] = "portIdentifier";
    static constexpr char VLAN_ID[] = "vlanIdentifier";
    static constexpr char OEM[] = "oem";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";
    static constexpr char TEST_PORT_ID[] = "TestPortId";
    static constexpr char TEST_VLAN_ID[] = "TestVlanId";

    virtual ~DeletePortVlanTest();
};

constexpr char DeletePortVlanTest::COMPONENT[];
constexpr char DeletePortVlanTest::PORT_ID[];
constexpr char DeletePortVlanTest::VLAN_ID[];
constexpr char DeletePortVlanTest::OEM[];
constexpr char DeletePortVlanTest::TEST_UUID[];
constexpr char DeletePortVlanTest::TEST_PORT_ID[];
constexpr char DeletePortVlanTest::TEST_VLAN_ID[];

DeletePortVlanTest::~DeletePortVlanTest() { }

TEST_F(DeletePortVlanTest, PositiveExecute) {
    network::json::DeletePortVlan command_json;
    DeletePortVlan* command = new DeletePortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = TEST_VLAN_ID;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[OEM].isObject());
}

TEST_F(DeletePortVlanTest, NegativeComponentNotFound) {
    network::json::DeletePortVlan command_json;
    DeletePortVlan* command = new DeletePortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = TEST_VLAN_ID;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(DeletePortVlanTest, NegativeInvalidUUIDFormat) {
    network::json::DeletePortVlan command_json;
    DeletePortVlan* command = new DeletePortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = TEST_VLAN_ID;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(DeletePortVlanTest, NegativePortNotFound) {
    network::json::DeletePortVlan command_json;
    DeletePortVlan* command = new DeletePortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_ID] = "OtherPortId";
    params[VLAN_ID] = TEST_VLAN_ID;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(DeletePortVlanTest, NegativeVlanNotFound) {
    network::json::DeletePortVlan command_json;
    DeletePortVlan* command = new DeletePortVlan(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_ID] = TEST_PORT_ID;
    params[VLAN_ID] = 2;

    EXPECT_ANY_THROW(command_json.method(params, result));
}
