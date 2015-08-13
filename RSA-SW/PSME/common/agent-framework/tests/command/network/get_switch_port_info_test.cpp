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

#include "agent-framework/command/network/get_switch_port_info.hpp"
#include "json/get_switch_port_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetSwitchPortInfo : public network::GetSwitchPortInfo {
private:
    std::string m_component{};
    std::string m_port_identifier{};
public:
    GetSwitchPortInfo(
        const std::string& component,
        const std::string& port_identifier) {
        m_component = component;
        m_port_identifier = port_identifier;
        }

    using network::GetSwitchPortInfo::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();
        auto port_identifier = request.get_port_identifier();

        if (component != m_component || port_identifier != m_port_identifier) {
            throw exception::NotFound();
        }

        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_port_identifier("Test Port Id");
        response.set_port_type("Test Port Type");
        response.set_link_technology("Test Link Technology");
        response.set_link_speed(1);
        response.set_max_speed(10);
        response.set_port_width(1);
        response.set_frame_size(1);
        response.set_autosense(true);
        response.set_is_management_port(true);
        response.set_last_error_code(1);
        response.set_error_cleared(true);
        response.set_last_state_change_time("Test State Change Time");
        response.set_mac_address("Test Mac Address");
        response.set_ipv4address(agent_framework::generic::IPv4Address());
        response.set_ipv6address(agent_framework::generic::IPv6Address());
        response.set_neighbor_info(agent_framework::generic::NeighborInfo("TestSwitchIdentifier", "TestPortIdentifier"));
        response.set_vlan_enable(true);
        response.set_oem(agent_framework::generic::OEMData());
    }

    virtual ~GetSwitchPortInfo();
};

GetSwitchPortInfo::~GetSwitchPortInfo() { }

class GetSwitchPortInfoTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char PORT_IDENTIFIER[] = "portIdentifier";
    static constexpr char STATUS[] = "status";
    static constexpr char STATE[] = "state";
    static constexpr char HEALTH[] = "health";
    static constexpr char LINK_TECHNOLOGY[] = "linkTechnology";
    static constexpr char LINK_SPEED_GBPS[] = "linkSpeedGbps";
    static constexpr char MAX_SPEED_GBPS[] = "maxSpeedGbps";
    static constexpr char OPERATIONAL_STATE[] = "operationalState";
    static constexpr char ADMINISTARTIVE_STATE[] = "administrativeState";
    static constexpr char PORT_WIDTH[] = "portWidth";
    static constexpr char FRAME_SIZE[] = "frameSize";
    static constexpr char AUTOSENSE[] = "autoSense";
    static constexpr char IS_MANAGEMENT_PORT[] = "isManagementPort";
    static constexpr char LAST_ERROR_CODE[] = "lastErrorCode";
    static constexpr char ERROR_CLEARED[] = "errorCleared";
    static constexpr char LAST_STATE_CHANGE_TIME[] = "lastStateChangeTime";
    static constexpr char MAC_ADDRESS[] = "macAddress";
    static constexpr char IPV4_ADDRESS[] = "ipv4address";
    static constexpr char IPV6_ADDRESS[] = "ipv6address";
    static constexpr char NEIGHBOR_INFO[] = "neighborInfo";
    static constexpr char SWITCH_IDENTIFIER[] = "switchIdentifier";
    static constexpr char VLAN_ENABLE[] = "vlanEnable";
    static constexpr char OEM[] = "oem";
    static constexpr char TEST_UUID[] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";
    static constexpr char TEST_PORT_ID[] = "TestPortId";

    virtual ~GetSwitchPortInfoTest();
};

constexpr char GetSwitchPortInfoTest::COMPONENT[];
constexpr char GetSwitchPortInfoTest::PORT_IDENTIFIER[];
constexpr char GetSwitchPortInfoTest::STATUS[];
constexpr char GetSwitchPortInfoTest::STATE[];
constexpr char GetSwitchPortInfoTest::HEALTH[];
constexpr char GetSwitchPortInfoTest::LINK_TECHNOLOGY[];
constexpr char GetSwitchPortInfoTest::LINK_SPEED_GBPS[];
constexpr char GetSwitchPortInfoTest::MAX_SPEED_GBPS[];
constexpr char GetSwitchPortInfoTest::OPERATIONAL_STATE[];
constexpr char GetSwitchPortInfoTest::ADMINISTARTIVE_STATE[];
constexpr char GetSwitchPortInfoTest::PORT_WIDTH[];
constexpr char GetSwitchPortInfoTest::FRAME_SIZE[];
constexpr char GetSwitchPortInfoTest::AUTOSENSE[];
constexpr char GetSwitchPortInfoTest::IS_MANAGEMENT_PORT[];
constexpr char GetSwitchPortInfoTest::LAST_ERROR_CODE[];
constexpr char GetSwitchPortInfoTest::ERROR_CLEARED[];
constexpr char GetSwitchPortInfoTest::LAST_STATE_CHANGE_TIME[];
constexpr char GetSwitchPortInfoTest::MAC_ADDRESS[];
constexpr char GetSwitchPortInfoTest::IPV4_ADDRESS[];
constexpr char GetSwitchPortInfoTest::IPV6_ADDRESS[];
constexpr char GetSwitchPortInfoTest::NEIGHBOR_INFO[];
constexpr char GetSwitchPortInfoTest::SWITCH_IDENTIFIER[];
constexpr char GetSwitchPortInfoTest::VLAN_ENABLE[];
constexpr char GetSwitchPortInfoTest::OEM[];
constexpr char GetSwitchPortInfoTest::TEST_UUID[];
constexpr char GetSwitchPortInfoTest::TEST_PORT_ID[];

GetSwitchPortInfoTest::~GetSwitchPortInfoTest() { }

TEST_F(GetSwitchPortInfoTest, PositiveExecute) {
    network::json::GetSwitchPortInfo command_json;
    GetSwitchPortInfo* command = new GetSwitchPortInfo(TEST_UUID, TEST_PORT_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_IDENTIFIER] = TEST_PORT_ID;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[STATUS].isObject());
    ASSERT_TRUE(result[STATUS][STATE].isString());
    ASSERT_TRUE(result[STATUS][HEALTH].isString());
    ASSERT_TRUE(result[PORT_IDENTIFIER].isString());
    ASSERT_TRUE(result[LINK_TECHNOLOGY].isString());
    ASSERT_TRUE(result[LINK_SPEED_GBPS].isUInt());
    ASSERT_TRUE(result[MAX_SPEED_GBPS].isUInt());
    ASSERT_TRUE(result[OPERATIONAL_STATE].isString());
    ASSERT_TRUE(result[ADMINISTARTIVE_STATE].isString());
    ASSERT_TRUE(result[PORT_WIDTH].isUInt());
    ASSERT_TRUE(result[FRAME_SIZE].isUInt());
    ASSERT_TRUE(result[AUTOSENSE].isBool());
    ASSERT_TRUE(result[IS_MANAGEMENT_PORT].isBool());
    ASSERT_TRUE(result[LAST_ERROR_CODE].isUInt());
    ASSERT_TRUE(result[ERROR_CLEARED].isBool());
    ASSERT_TRUE(result[LAST_STATE_CHANGE_TIME].isString());
    ASSERT_TRUE(result[MAC_ADDRESS].isString());
    ASSERT_TRUE(result[IPV4_ADDRESS].isObject());
    ASSERT_TRUE(result[IPV6_ADDRESS].isObject());
    ASSERT_TRUE(result[NEIGHBOR_INFO].isObject());
    ASSERT_TRUE(result[VLAN_ENABLE].isBool());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_EQ(result[STATUS][STATE], "TestState");
    ASSERT_EQ(result[STATUS][HEALTH], "TestHealth");
    ASSERT_EQ(result[PORT_IDENTIFIER].asString(), "Test Port Id");
    ASSERT_EQ(result[LINK_TECHNOLOGY].asString(), "Test Link Technology");
    ASSERT_EQ(result[LINK_SPEED_GBPS].asUInt(), 1);
    ASSERT_EQ(result[MAX_SPEED_GBPS].asUInt(), 10);
    ASSERT_EQ(result[OPERATIONAL_STATE].asString(), "Unknown");
    ASSERT_EQ(result[ADMINISTARTIVE_STATE].asString(), "None");
    ASSERT_EQ(result[PORT_WIDTH].asUInt(), 1);
    ASSERT_EQ(result[FRAME_SIZE].asUInt(), 1);
    ASSERT_EQ(result[AUTOSENSE].asBool(), true);
    ASSERT_EQ(result[IS_MANAGEMENT_PORT].asBool(), true);
    ASSERT_EQ(result[LAST_ERROR_CODE].asUInt(), 1);
    ASSERT_EQ(result[ERROR_CLEARED].asBool(), true);
    ASSERT_EQ(result[LAST_STATE_CHANGE_TIME].asString(), "Test State Change Time");
    ASSERT_EQ(result[MAC_ADDRESS].asString(), "Test Mac Address");
    ASSERT_EQ(result[NEIGHBOR_INFO][SWITCH_IDENTIFIER].asString(), "TestSwitchIdentifier");
    ASSERT_EQ(result[NEIGHBOR_INFO][PORT_IDENTIFIER].asString(), "TestPortIdentifier");
    ASSERT_EQ(result[VLAN_ENABLE].asBool(), true);

}

TEST_F(GetSwitchPortInfoTest, NegativeComponentNotFound) {
    network::json::GetSwitchPortInfo command_json;
    GetSwitchPortInfo* command = new GetSwitchPortInfo(TEST_UUID, TEST_PORT_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";
    params[PORT_IDENTIFIER] = TEST_PORT_ID;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetSwitchPortInfoTest, NegativePortIdentifierNotFound) {
    network::json::GetSwitchPortInfo command_json;
    GetSwitchPortInfo* command = new GetSwitchPortInfo(TEST_UUID, TEST_PORT_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;
    params[PORT_IDENTIFIER] = "OtherTestPortId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetSwitchPortInfoTest, NegativeInvalidUUIDFormat) {
    network::json::GetSwitchPortInfo command_json;
    GetSwitchPortInfo* command = new GetSwitchPortInfo(TEST_UUID, TEST_PORT_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";
    params[PORT_IDENTIFIER] = TEST_PORT_ID;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
