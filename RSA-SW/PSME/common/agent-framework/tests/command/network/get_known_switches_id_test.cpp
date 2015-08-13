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

#include "agent-framework/command/network/get_known_switches_id.hpp"
#include "json/get_known_switches_id.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetKnownSwitchesId : public network::GetKnownSwitchesId {
private:
    std::string m_component{};
public:
    GetKnownSwitchesId(const std::string& component) { m_component = component; }

    using network::GetKnownSwitchesId::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();

        if (component != m_component) {
            throw exception::NotFound();
        }

        for (std::uint32_t i = 0; i < 10; ++i) {
            response.add_switch_id(std::to_string(i));
        }
    }
    virtual ~GetKnownSwitchesId();
};

GetKnownSwitchesId::~GetKnownSwitchesId() { }

class GetKnownSwitchesIdTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char SWITCH_IDENTIFIERS[] = "switchIdentifiers";
    static constexpr char ID[] = "Id";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";

    virtual ~GetKnownSwitchesIdTest();
};

constexpr char GetKnownSwitchesIdTest::COMPONENT[];
constexpr char GetKnownSwitchesIdTest::SWITCH_IDENTIFIERS[];
constexpr char GetKnownSwitchesIdTest::ID[];
constexpr char GetKnownSwitchesIdTest::TEST_UUID[];

GetKnownSwitchesIdTest::~GetKnownSwitchesIdTest() { }

TEST_F(GetKnownSwitchesIdTest, PositiveExecute) {
    network::json::GetKnownSwitchesId command_json;
    GetKnownSwitchesId* command = new GetKnownSwitchesId(TEST_UUID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    for (std::uint32_t i = 0; i < 10; ++i) {
        ASSERT_EQ(result[SWITCH_IDENTIFIERS][i][ID].asString(), std::to_string(i));
    }
}

TEST_F(GetKnownSwitchesIdTest, NegativeComponentNotFound) {
    network::json::GetKnownSwitchesId command_json;
    GetKnownSwitchesId* command = new GetKnownSwitchesId(TEST_UUID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetKnownSwitchesIdTest, NegativeInvalidUUIDFormat) {
    network::json::GetKnownSwitchesId command_json;
    GetKnownSwitchesId* command = new GetKnownSwitchesId(TEST_UUID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
