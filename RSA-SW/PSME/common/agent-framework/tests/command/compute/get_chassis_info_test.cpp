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

#include "agent-framework/command/compute/get_chassis_info.hpp"
#include "json/get_chassis_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetChassisInfo : public compute::GetChassisInfo {
private:
    std::string m_chassis{};
public:
    GetChassisInfo(const std::string& chassis) { m_chassis = chassis; }

    using compute::GetChassisInfo::execute;

    void execute(const Request& request, Response& response) {
        auto chassis = request.get_chassis();

        if (chassis != m_chassis) {
            throw exception::NotFound();
        }

        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_type("Test Type");
        response.set_size(2048);
        response.set_location_offset(1);
        response.set_oem(agent_framework::generic::OEMData());
        response.set_fru_info(agent_framework::generic::FruInfo(
          "TestSerialNumber", "TestManufacturer", "TestModelNumber", "TestPartNumber"));
    }

    virtual ~GetChassisInfo();
};

GetChassisInfo::~GetChassisInfo() { }

class GetChassisInfoTest : public ::testing::Test {
public:
    virtual ~GetChassisInfoTest();
};

GetChassisInfoTest::~GetChassisInfoTest() { }

TEST_F(GetChassisInfoTest, PositiveExecute) {
    compute::json::GetChassisInfo command_json;
    GetChassisInfo* command = new GetChassisInfo("TestChassis");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["chassis"] = "TestChassis";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result["type"].isString());
    ASSERT_TRUE(result["size"].isUInt());
    ASSERT_TRUE(result["locationOffset"].isUInt());
    ASSERT_TRUE(result["status"].isObject());
    ASSERT_TRUE(result["status"]["state"].isString());
    ASSERT_TRUE(result["status"]["health"].isString());
    ASSERT_TRUE(result["oem"].isObject());
    ASSERT_TRUE(result["fruInfo"].isObject());
    ASSERT_TRUE(result["fruInfo"]["serialNumber"].isString());
    ASSERT_TRUE(result["fruInfo"]["manufacturer"].isString());
    ASSERT_TRUE(result["fruInfo"]["modelNumber"].isString());
    ASSERT_TRUE(result["fruInfo"]["partNumber"].isString());
    ASSERT_EQ(result["type"].asString(), "Test Type");
    ASSERT_EQ(result["size"].asUInt(), 2048);
    ASSERT_EQ(result["locationOffset"].asUInt(), 1);
    ASSERT_EQ(result["status"]["state"], "TestState");
    ASSERT_EQ(result["status"]["health"], "TestHealth");
    ASSERT_EQ(result["fruInfo"]["serialNumber"], "TestSerialNumber");
    ASSERT_EQ(result["fruInfo"]["manufacturer"], "TestManufacturer");
    ASSERT_EQ(result["fruInfo"]["modelNumber"], "TestModelNumber");
    ASSERT_EQ(result["fruInfo"]["partNumber"], "TestPartNumber");
}

TEST_F(GetChassisInfoTest, NegativeModuleNotFound) {
    compute::json::GetChassisInfo command_json;
    GetChassisInfo* command = new GetChassisInfo("TestChassis");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["chassis"] = "OtherTestChassis";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

