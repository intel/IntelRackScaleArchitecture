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

#include "agent-framework/command/network/get_neighbor_switches_id.hpp"
#include "json/get_neighbor_switches_id.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetNeighborSwitchesId : public network::GetNeighborSwitchesId {
private:
    std::string m_component{};
    std::string m_switch_identifier{};
public:
    GetNeighborSwitchesId(
        const std::string& component,
        const std::string& switch_identifier) {
        m_component = component;
        m_switch_identifier = switch_identifier;
        }

    using network::GetNeighborSwitchesId::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();
        auto switch_identifier = request.get_switch_id();

        if (component != m_component || switch_identifier != m_switch_identifier) {
            throw exception::NotFound();
        }

        for (std::uint32_t i = 0; i < 10; ++i) {
            response.add_switch_id(std::to_string(i));
        }
    }
    virtual ~GetNeighborSwitchesId();
};

GetNeighborSwitchesId::~GetNeighborSwitchesId() { }

class GetNeighborSwitchesIdTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char SWITCH_IDENTIFIER[] = "switchIdentifier";
    static constexpr char SWITCH_IDENTIFIERS[] = "switchIdentifiers";
    static constexpr char ID[] = "Id";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";
    static constexpr char TEST_SWITCH_IDENTIFIER[] = "TestSwitchIdentifier";

    virtual ~GetNeighborSwitchesIdTest();
};

constexpr char GetNeighborSwitchesIdTest::COMPONENT[];
constexpr char GetNeighborSwitchesIdTest::SWITCH_IDENTIFIER[];
constexpr char GetNeighborSwitchesIdTest::SWITCH_IDENTIFIERS[];
constexpr char GetNeighborSwitchesIdTest::ID[];
constexpr char GetNeighborSwitchesIdTest::TEST_UUID[];
constexpr char GetNeighborSwitchesIdTest::TEST_SWITCH_IDENTIFIER[];

GetNeighborSwitchesIdTest::~GetNeighborSwitchesIdTest() { }

TEST_F(GetNeighborSwitchesIdTest, PositiveExecute) {
    network::json::GetNeighborSwitchesId command_json;
    GetNeighborSwitchesId* command = new GetNeighborSwitchesId(TEST_UUID, TEST_SWITCH_IDENTIFIER);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[SWITCH_IDENTIFIER] = TEST_SWITCH_IDENTIFIER;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    for (std::uint32_t i = 0; i < 10; ++i) {
        ASSERT_EQ(result[SWITCH_IDENTIFIERS][i][ID].asString(), std::to_string(i));
    }
}

TEST_F(GetNeighborSwitchesIdTest, NegativeComponentNotFound) {
    network::json::GetNeighborSwitchesId command_json;
    GetNeighborSwitchesId* command = new GetNeighborSwitchesId(TEST_UUID, TEST_SWITCH_IDENTIFIER);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";
    params[SWITCH_IDENTIFIER] = TEST_SWITCH_IDENTIFIER;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetNeighborSwitchesIdTest, NegativeInvalidUUIDFormat) {
    network::json::GetNeighborSwitchesId command_json;
    GetNeighborSwitchesId* command = new GetNeighborSwitchesId(TEST_UUID, TEST_SWITCH_IDENTIFIER);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";
    params[SWITCH_IDENTIFIER] = TEST_SWITCH_IDENTIFIER;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetNeighborSwitchesIdTest, NegativeSwitchIdNotFound) {
    network::json::GetNeighborSwitchesId command_json;
    GetNeighborSwitchesId* command = new GetNeighborSwitchesId(TEST_UUID, TEST_SWITCH_IDENTIFIER);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[SWITCH_IDENTIFIER] = "OtherSwitchIdentifier";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
