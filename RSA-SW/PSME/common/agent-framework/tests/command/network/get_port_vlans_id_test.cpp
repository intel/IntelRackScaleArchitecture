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

#include "agent-framework/command/network/get_port_vlans_id.hpp"
#include "json/get_port_vlans_id.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetPortVlansId : public network::GetPortVlansId {
private:
    std::string m_component{};
    std::string m_port_identifier{};
public:
    GetPortVlansId(
        const std::string& component,
        const std::string& port_identifier) {
        m_component = component;
        m_port_identifier = port_identifier;
        }

    using network::GetPortVlansId::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();
        auto port_identifier = request.get_port_identifier();

        if (component != m_component || port_identifier != m_port_identifier) {
            throw exception::NotFound();
        }

        for (std::uint32_t i = 0; i < 10; ++i) {
            response.add_vlan_id(std::to_string(i));
        }
    }

    virtual ~GetPortVlansId();
};

GetPortVlansId::~GetPortVlansId() { }

class GetPortVlansIdTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char PORT_IDENTIFIER[] = "portIdentifier";
    static constexpr char VLAN_IDENTIFIERS[] = "vlanIdentifiers";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";
    static constexpr char TEST_PORT_ID[] = "TestPortId";
    static constexpr char ID[] = "Id";

    virtual ~GetPortVlansIdTest();
};

constexpr char GetPortVlansIdTest::COMPONENT[];
constexpr char GetPortVlansIdTest::PORT_IDENTIFIER[];
constexpr char GetPortVlansIdTest::VLAN_IDENTIFIERS[];
constexpr char GetPortVlansIdTest::TEST_UUID[];
constexpr char GetPortVlansIdTest::TEST_PORT_ID[];
constexpr char GetPortVlansIdTest::ID[];

GetPortVlansIdTest::~GetPortVlansIdTest() { }

TEST_F(GetPortVlansIdTest, PositiveExecute) {
    network::json::GetPortVlansId command_json;
    GetPortVlansId* command = new GetPortVlansId(TEST_UUID, TEST_PORT_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_IDENTIFIER] = TEST_PORT_ID;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    for (std::uint32_t i = 0; i < 10; ++i) {
        ASSERT_EQ(result[VLAN_IDENTIFIERS][i][ID].asString(), std::to_string(i));
    }
}

TEST_F(GetPortVlansIdTest, NegativeComponentNotFound) {
    network::json::GetPortVlansId command_json;
    GetPortVlansId* command = new GetPortVlansId(TEST_UUID, TEST_PORT_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";
    params[PORT_IDENTIFIER] = TEST_PORT_ID;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetPortVlansIdTest, NegativePortIdentifierNotFound) {
    network::json::GetPortVlansId command_json;
    GetPortVlansId* command = new GetPortVlansId(TEST_UUID, TEST_PORT_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_IDENTIFIER] = "OtherTestPortId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetPortVlansIdTest, NegativeInvalidUUIDFormat) {
    network::json::GetPortVlansId command_json;
    GetPortVlansId* command = new GetPortVlansId(TEST_UUID, TEST_PORT_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";
    params[PORT_IDENTIFIER] = TEST_PORT_ID;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
