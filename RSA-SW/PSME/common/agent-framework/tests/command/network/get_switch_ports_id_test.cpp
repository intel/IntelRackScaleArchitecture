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

#include "agent-framework/command/network/get_switch_ports_id.hpp"
#include "json/get_switch_ports_id.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetSwitchPortsId : public network::GetSwitchPortsId {
private:
    std::string m_component{};
public:
    GetSwitchPortsId(const std::string& component) { m_component = component; }

    using network::GetSwitchPortsId::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();

        if (component != m_component) {
            throw exception::NotFound();
        }

        for (std::uint32_t i = 0; i < 10; ++i) {
            response.add_port_id(std::to_string(i));
        }
    }
    virtual ~GetSwitchPortsId();
};

GetSwitchPortsId::~GetSwitchPortsId() { }

class GetSwitchPortsIdTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char PORT_IDENTIFIERS[] = "portIdentifiers";
    static constexpr char ID[] = "Id";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";

    virtual ~GetSwitchPortsIdTest();
};

constexpr char GetSwitchPortsIdTest::COMPONENT[];
constexpr char GetSwitchPortsIdTest::PORT_IDENTIFIERS[];
constexpr char GetSwitchPortsIdTest::ID[];
constexpr char GetSwitchPortsIdTest::TEST_UUID[];

GetSwitchPortsIdTest::~GetSwitchPortsIdTest() { }

TEST_F(GetSwitchPortsIdTest, PositiveExecute) {
    network::json::GetSwitchPortsId command_json;
    GetSwitchPortsId* command = new GetSwitchPortsId(TEST_UUID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    for (std::uint32_t i = 0; i < 10; ++i) {
        ASSERT_EQ(result[PORT_IDENTIFIERS][i][ID].asString(), std::to_string(i));
    }
}

TEST_F(GetSwitchPortsIdTest, NegativeComponentNotFound) {
    network::json::GetSwitchPortsId command_json;
    GetSwitchPortsId* command = new GetSwitchPortsId(TEST_UUID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetSwitchPortsIdTest, NegativeInvalidUUIDFormat) {
    network::json::GetSwitchPortsId command_json;
    GetSwitchPortsId* command = new GetSwitchPortsId(TEST_UUID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
