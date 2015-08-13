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

#include "agent-framework/command/network/get_remote_switch_info.hpp"
#include "json/get_remote_switch_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetRemoteSwitchInfo : public network::GetRemoteSwitchInfo {
private:
    std::string m_component{};
    std::string m_switch_identifier{};
public:
    GetRemoteSwitchInfo(
        const std::string& component,
        const std::string& switch_identifier) {
        m_component = component;
        m_switch_identifier = switch_identifier;
        }

    using network::GetRemoteSwitchInfo::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();
        auto switch_identifier = request.get_switch_id();

        if (component != m_component || switch_identifier != m_switch_identifier) {
            throw exception::NotFound();
        }

        response.set_mac_address("Test Mac Address");

        for (std::uint32_t i = 0; i < 10; ++i) {
            response.add_next_hop(Response::Hop(std::to_string(i), "127.0.0.1", "FE80::0202:B3FF:FE1E:8329", 1));
        }
    }

    virtual ~GetRemoteSwitchInfo();
};

GetRemoteSwitchInfo::~GetRemoteSwitchInfo() { }

class GetRemoteSwitchInfoTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char SWITCH_IDENTIFIER[] = "switchIdentifier";
    static constexpr char MAC_ADDRESS[] = "macAddress";
    static constexpr char NEXT_HOP[] = "nextHop";
    static constexpr char PORT_IDENTIFIER[] = "portIdentifier";
    static constexpr char IPV4_ADDRESS[] = "ipv4Address";
    static constexpr char IPV6_ADDRESS[] = "ipv6Address";
    static constexpr char METRIC[] = "metric";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";
    static constexpr char TEST_SWITCH_ID[] = "TestSwitchId";

    virtual ~GetRemoteSwitchInfoTest();
};

constexpr char GetRemoteSwitchInfoTest::COMPONENT[];
constexpr char GetRemoteSwitchInfoTest::SWITCH_IDENTIFIER[];
constexpr char GetRemoteSwitchInfoTest::MAC_ADDRESS[];
constexpr char GetRemoteSwitchInfoTest::NEXT_HOP[];
constexpr char GetRemoteSwitchInfoTest::PORT_IDENTIFIER[];
constexpr char GetRemoteSwitchInfoTest::IPV4_ADDRESS[];
constexpr char GetRemoteSwitchInfoTest::IPV6_ADDRESS[];
constexpr char GetRemoteSwitchInfoTest::METRIC[];
constexpr char GetRemoteSwitchInfoTest::TEST_UUID[];
constexpr char GetRemoteSwitchInfoTest::TEST_SWITCH_ID[];

GetRemoteSwitchInfoTest::~GetRemoteSwitchInfoTest() { }

TEST_F(GetRemoteSwitchInfoTest, PositiveExecute) {
    network::json::GetRemoteSwitchInfo command_json;
    GetRemoteSwitchInfo* command = new GetRemoteSwitchInfo(TEST_UUID, TEST_SWITCH_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[SWITCH_IDENTIFIER] = TEST_SWITCH_ID;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[NEXT_HOP].isArray());
    ASSERT_TRUE(result[MAC_ADDRESS].isString());
    ASSERT_TRUE(result[NEXT_HOP][0][PORT_IDENTIFIER].isString());
    ASSERT_TRUE(result[NEXT_HOP][0][IPV4_ADDRESS].isString());
    ASSERT_TRUE(result[NEXT_HOP][0][IPV6_ADDRESS].isString());
    ASSERT_TRUE(result[NEXT_HOP][0][METRIC].isInt());
    ASSERT_EQ(result[MAC_ADDRESS].asString(), "Test Mac Address");
    for (std::uint32_t i = 0; i < 10; ++i) {
        ASSERT_EQ(result[NEXT_HOP][i][PORT_IDENTIFIER].asString(), std::to_string(i));
        ASSERT_EQ(result[NEXT_HOP][i][IPV4_ADDRESS].asString(), "127.0.0.1");
        ASSERT_EQ(result[NEXT_HOP][i][IPV6_ADDRESS].asString(), "FE80::0202:B3FF:FE1E:8329");
        ASSERT_EQ(result[NEXT_HOP][i][METRIC].asInt(), 1);
    }
}

TEST_F(GetRemoteSwitchInfoTest, NegativeComponentNotFound) {
    network::json::GetRemoteSwitchInfo command_json;
    GetRemoteSwitchInfo* command = new GetRemoteSwitchInfo(TEST_UUID, TEST_SWITCH_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";
    params[SWITCH_IDENTIFIER] = TEST_SWITCH_ID;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetRemoteSwitchInfoTest, NegativeSwitchIdentifierNotFound) {
    network::json::GetRemoteSwitchInfo command_json;
    GetRemoteSwitchInfo* command = new GetRemoteSwitchInfo(TEST_UUID, TEST_SWITCH_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[SWITCH_IDENTIFIER] = "OtherTestSwitchId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetRemoteSwitchInfoTest, NegativeInvalidUUIDFormat) {
    network::json::GetRemoteSwitchInfo command_json;
    GetRemoteSwitchInfo* command = new GetRemoteSwitchInfo(TEST_UUID, TEST_SWITCH_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";
    params[SWITCH_IDENTIFIER] = TEST_SWITCH_ID;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
