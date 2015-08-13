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

#include "agent-framework/command/compute/get_blade_info.hpp"
#include "json/get_blade_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetBladeInfo : public compute::GetBladeInfo {
private:
    std::string m_component{};
public:
    GetBladeInfo(const std::string& component) { m_component = component; }

    using compute::GetBladeInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();

        if (uuid != m_component) {
            throw exception::NotFound();
        }

        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_bios_version("Test Bios Version");
        response.set_boot_override("Test Boot Override");
        response.set_boot_override_target("Test Boot Override Target");
        response.set_boot_override_supported({"One", "Two"});
        response.set_processor_count(1);
        response.set_dimm_count(1);
        response.set_controller_count(1);
        response.set_nic_count(1);
        response.set_power_state("On");
        response.set_slot(1);
        response.set_fru_info(agent_framework::generic::FruInfo(
          "TestSerialNumber", "TestManufacturer", "TestModelNumber", "TestPartNumber"));
    }

    virtual ~GetBladeInfo();
};

GetBladeInfo::~GetBladeInfo() { }

class GetBladeInfoTest : public ::testing::Test {
public:
    virtual ~GetBladeInfoTest();
};

GetBladeInfoTest::~GetBladeInfoTest() { }

TEST_F(GetBladeInfoTest, PositiveExecute) {
    compute::json::GetBladeInfo command_json;
    GetBladeInfo* command = new GetBladeInfo("TestModuleId");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "TestModuleId";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result["status"].isObject());
    ASSERT_TRUE(result["status"]["state"].isString());
    ASSERT_TRUE(result["status"]["health"].isString());
    ASSERT_TRUE(result["biosVersion"].isString());
    ASSERT_TRUE(result["bootOverride"].isString());
    ASSERT_TRUE(result["bootOverrideTarget"].isString());
    ASSERT_TRUE(result["bootOverrideSupported"].isArray());
    ASSERT_TRUE(result["bootOverrideSupported"][0].isString());
    ASSERT_TRUE(result["bootOverrideSupported"][1].isString());
    ASSERT_TRUE(result["processorCount"].isUInt());
    ASSERT_TRUE(result["dimmCount"].isUInt());
    ASSERT_TRUE(result["controllerCount"].isUInt());
    ASSERT_TRUE(result["nicCount"].isUInt());
    ASSERT_TRUE(result["powerState"].isString());
    ASSERT_TRUE(result["slot"].isUInt());
    ASSERT_TRUE(result["fruInfo"].isObject());
    ASSERT_TRUE(result["fruInfo"]["serialNumber"].isString());
    ASSERT_TRUE(result["fruInfo"]["manufacturer"].isString());
    ASSERT_TRUE(result["fruInfo"]["modelNumber"].isString());
    ASSERT_TRUE(result["fruInfo"]["partNumber"].isString());
    ASSERT_EQ(result["status"]["state"], "TestState");
    ASSERT_EQ(result["status"]["health"], "TestHealth");
    ASSERT_EQ(result["biosVersion"].asString(), "Test Bios Version");
    ASSERT_EQ(result["bootOverride"].asString(), "Test Boot Override");
    ASSERT_EQ(result["bootOverrideTarget"].asString(), "Test Boot Override Target");
    ASSERT_EQ(result["bootOverrideSupported"][0].asString(), "One");
    ASSERT_EQ(result["bootOverrideSupported"][1].asString(), "Two");
    ASSERT_EQ(result["processorCount"].asUInt(), 1);
    ASSERT_EQ(result["dimmCount"].asUInt(), 1);
    ASSERT_EQ(result["controllerCount"].asUInt(), 1);
    ASSERT_EQ(result["nicCount"].asUInt(), 1);
    ASSERT_EQ(result["powerState"].asString(), "On");
    ASSERT_EQ(result["slot"].asUInt(), 1);
    ASSERT_EQ(result["fruInfo"]["serialNumber"], "TestSerialNumber");
    ASSERT_EQ(result["fruInfo"]["manufacturer"], "TestManufacturer");
    ASSERT_EQ(result["fruInfo"]["modelNumber"], "TestModelNumber");
    ASSERT_EQ(result["fruInfo"]["partNumber"], "TestPartNumber");
}

TEST_F(GetBladeInfoTest, NegativeModuleNotFound) {
    compute::json::GetBladeInfo command_json;
    GetBladeInfo* command = new GetBladeInfo("TestModuleId");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "OtherTestModuleId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
