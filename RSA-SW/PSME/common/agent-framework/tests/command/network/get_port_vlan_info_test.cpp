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

#include "agent-framework/command/network/get_port_vlan_info.hpp"
#include "json/get_port_vlan_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetPortVlanInfo : public network::GetPortVlanInfo {
private:
    std::string m_component{};
    std::string m_port_identifier{};
    std::string m_vlan_identifier{};
public:
    GetPortVlanInfo(
        const std::string& component,
        const std::string& port_identifier,
        const std::string& vlan_identifier) {
        m_component = component;
        m_port_identifier = port_identifier;
        m_vlan_identifier = vlan_identifier;
        }

    using network::GetPortVlanInfo::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();
        auto port_identifier = request.get_port_identifier();
        auto vlan_identifier = request.get_vlan_identifier();

        if (component != m_component
            || port_identifier != m_port_identifier
            || vlan_identifier != m_vlan_identifier) {
            throw exception::NotFound();
        }

        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_vlan_id(1);
        response.set_vlan_enable(true);
        response.set_tagged(true);
        response.set_ipv4address(agent_framework::generic::IPv4Address());
        response.set_ipv6address(agent_framework::generic::IPv6Address());
        response.set_oem(agent_framework::generic::OEMData());
    }

    virtual ~GetPortVlanInfo();
};

GetPortVlanInfo::~GetPortVlanInfo() { }

class GetPortVlanInfoTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char PORT_IDENTIFIER[] = "portIdentifier";
    static constexpr char VLAN_IDENTIFIER[] = "vlanIdentifier";
    static constexpr char STATUS[] = "status";
    static constexpr char STATE[] = "state";
    static constexpr char HEALTH[] = "health";
    static constexpr char VLAN_ID[] = "vlanId";
    static constexpr char VLAN_ENABLE[] = "vlanEnable";
    static constexpr char TAGGED[] = "tagged";
    static constexpr char IPV4_ADDRESS[] = "ipv4address";
    static constexpr char IPV6_ADDRESS[] = "ipv6address";
    static constexpr char OEM[] = "oem";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";
    static constexpr char TEST_PORT_ID[] = "TestPortId";
    static constexpr char TEST_VLAN_ID[] = "TestVlanId";

    virtual ~GetPortVlanInfoTest();
};

constexpr char GetPortVlanInfoTest::COMPONENT[];
constexpr char GetPortVlanInfoTest::PORT_IDENTIFIER[];
constexpr char GetPortVlanInfoTest::VLAN_IDENTIFIER[];
constexpr char GetPortVlanInfoTest::STATUS[];
constexpr char GetPortVlanInfoTest::STATE[];
constexpr char GetPortVlanInfoTest::HEALTH[];
constexpr char GetPortVlanInfoTest::VLAN_ID[];
constexpr char GetPortVlanInfoTest::VLAN_ENABLE[];
constexpr char GetPortVlanInfoTest::TAGGED[];
constexpr char GetPortVlanInfoTest::IPV4_ADDRESS[];
constexpr char GetPortVlanInfoTest::IPV6_ADDRESS[];
constexpr char GetPortVlanInfoTest::OEM[];
constexpr char GetPortVlanInfoTest::TEST_UUID[];
constexpr char GetPortVlanInfoTest::TEST_PORT_ID[];
constexpr char GetPortVlanInfoTest::TEST_VLAN_ID[];

GetPortVlanInfoTest::~GetPortVlanInfoTest() { }

TEST_F(GetPortVlanInfoTest, PositiveExecute) {
    network::json::GetPortVlanInfo command_json;
    GetPortVlanInfo* command = new GetPortVlanInfo(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_IDENTIFIER] = TEST_PORT_ID;
    params[VLAN_IDENTIFIER] = TEST_VLAN_ID;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[STATUS].isObject());
    ASSERT_TRUE(result[STATUS][STATE].isString());
    ASSERT_TRUE(result[STATUS][HEALTH].isString());
    ASSERT_TRUE(result[VLAN_ID].isUInt());
    ASSERT_TRUE(result[VLAN_ENABLE].isBool());
    ASSERT_TRUE(result[TAGGED].isBool());
    ASSERT_TRUE(result[IPV4_ADDRESS].isObject());
    ASSERT_TRUE(result[IPV6_ADDRESS].isObject());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_EQ(result[STATUS][STATE], "TestState");
    ASSERT_EQ(result[STATUS][HEALTH], "TestHealth");
    ASSERT_EQ(result[VLAN_ID].asUInt(), 1);
    ASSERT_EQ(result[VLAN_ENABLE].asBool(), true);
    ASSERT_EQ(result[TAGGED].asBool(), true);

}

TEST_F(GetPortVlanInfoTest, NegativeComponentNotFound) {
    network::json::GetPortVlanInfo command_json;
    GetPortVlanInfo* command = new GetPortVlanInfo(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";
    params[PORT_IDENTIFIER] = TEST_PORT_ID;
    params[VLAN_IDENTIFIER] = TEST_VLAN_ID;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetPortVlanInfoTest, NegativePortIdentifierNotFound) {
    network::json::GetPortVlanInfo command_json;
    GetPortVlanInfo* command = new GetPortVlanInfo(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_IDENTIFIER] = "OtherTestPortId";
    params[VLAN_IDENTIFIER] = TEST_VLAN_ID;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetPortVlanInfoTest, NegativeVlanIdentifierNotFound) {
    network::json::GetPortVlanInfo command_json;
    GetPortVlanInfo* command = new GetPortVlanInfo(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_IDENTIFIER] = TEST_PORT_ID;
    params[VLAN_IDENTIFIER] = "OtherTestVlanId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetPortVlanInfoTest, NegativeInvalidUUIDFormat) {
    network::json::GetPortVlanInfo command_json;
    GetPortVlanInfo* command = new GetPortVlanInfo(TEST_UUID, TEST_PORT_ID, TEST_VLAN_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";
    params[PORT_IDENTIFIER] = TEST_PORT_ID;
    params[VLAN_IDENTIFIER] = TEST_VLAN_ID;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
