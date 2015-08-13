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

#include "agent-framework/command/compute/get_network_interface_info.hpp"
#include "json/get_network_interface_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetNetworkInterfaceInfo : public compute::GetNetworkInterfaceInfo {
private:
    std::string m_component{};
    unsigned int m_interface{};
public:
    GetNetworkInterfaceInfo(
        const std::string& component,
        std::uint32_t interface) {
        m_component = component;
        m_interface = interface;
    }

    using compute::GetNetworkInterfaceInfo::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();
        auto interface = request.get_interface();

        if (component != m_component || interface != m_interface) {
            throw exception::NotFound();
        }

        response.set_frame_size(2048);
        response.set_link_technology("Test Link Technology");
        response.set_speed_gbps(1);
        response.set_autosense(true);
        response.set_mac_address("Test");
        response.set_vlan_enable(true);
        response.set_vlan_count(1);
        response.set_ipv4_address(agent_framework::generic::IPv4Address());
        response.set_ipv6_address(agent_framework::generic::IPv6Address());
        response.set_neighbor_info(agent_framework::generic::NeighborInfo("TestSwitchIdentifier", "TestPortIdentifier"));
        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
    }

    virtual ~GetNetworkInterfaceInfo();
};

GetNetworkInterfaceInfo::~GetNetworkInterfaceInfo() { }

class GetNetworkInterfaceInfoTest : public ::testing::Test {
public:
    virtual ~GetNetworkInterfaceInfoTest();
};

GetNetworkInterfaceInfoTest::~GetNetworkInterfaceInfoTest() { }

TEST_F(GetNetworkInterfaceInfoTest, PositiveExecute) {
    compute::json::GetNetworkInterfaceInfo command_json;
    GetNetworkInterfaceInfo* command = new GetNetworkInterfaceInfo("TestModuleId", 1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "TestModuleId";
    params["interface"] = 1;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result["frameSize"].isUInt());
    ASSERT_TRUE(result["linkTechnology"].isString());
    ASSERT_TRUE(result["speedGbps"].isUInt());
    ASSERT_TRUE(result["autoSense"].isBool());
    ASSERT_TRUE(result["macAddress"].isString());
    ASSERT_TRUE(result["vlanEnable"].isBool());
    ASSERT_TRUE(result["vlanCount"].isUInt());
    ASSERT_TRUE(result["ipv4Address"].isObject());
    ASSERT_TRUE(result["ipv6Address"].isObject());
    ASSERT_TRUE(result["neighborInfo"].isObject());
    ASSERT_TRUE(result["status"].isObject());
    ASSERT_EQ(result["frameSize"].asUInt(), 2048);
    ASSERT_EQ(result["linkTechnology"].asString(), "Test Link Technology");
    ASSERT_EQ(result["speedGbps"].asUInt(), 1);
    ASSERT_EQ(result["autoSense"].asBool(), true);
    ASSERT_EQ(result["macAddress"].asString(), "Test");
    ASSERT_EQ(result["vlanEnable"].asBool(), true);
    ASSERT_EQ(result["vlanCount"].asUInt(), 1);
    ASSERT_EQ(result["status"]["state"], "TestState");
    ASSERT_EQ(result["status"]["health"], "TestHealth");
}

TEST_F(GetNetworkInterfaceInfoTest, NegativeModuleNotFound) {
    compute::json::GetNetworkInterfaceInfo command_json;
    GetNetworkInterfaceInfo* command = new GetNetworkInterfaceInfo("TestModuleId", 1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "OtherTestModuleId";
    params["interface"] = 1;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetNetworkInterfaceInfoTest, NegativeInterfaceNotFound) {
    compute::json::GetNetworkInterfaceInfo command_json;
    GetNetworkInterfaceInfo* command = new GetNetworkInterfaceInfo("TestModuleId", 1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "TestModuleId";
    params["interface"] = 2;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
