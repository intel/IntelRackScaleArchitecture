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

#include "agent-framework/command/compute/get_storage_controller_info.hpp"
#include "json/get_storage_controller_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetStorageControllerInfo : public compute::GetStorageControllerInfo {
private:
    std::string m_component{};
    unsigned int m_controller{};
public:
    GetStorageControllerInfo(
        const std::string& component,
        std::uint32_t controller) {
        m_component = component;
        m_controller = controller;
        }

    using compute::GetStorageControllerInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();
        auto controller = request.get_controller();

        if (uuid != m_component || controller != m_controller) {
            throw exception::NotFound();
        }

        response.set_interface("Test Interface");
        response.set_drive_count(2);
        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_fru_info(agent_framework::generic::FruInfo(
          "TestSerialNumber", "TestManufacturer", "TestModelNumber", "TestPartNumber"));
    }

    virtual ~GetStorageControllerInfo();
};

GetStorageControllerInfo::~GetStorageControllerInfo() { }

class GetStorageControllerInfoTest : public ::testing::Test {
public:
    virtual ~GetStorageControllerInfoTest();
};

GetStorageControllerInfoTest::~GetStorageControllerInfoTest() { }

TEST_F(GetStorageControllerInfoTest, PositiveExecute) {
    compute::json::GetStorageControllerInfo command_json;
    GetStorageControllerInfo* command = new GetStorageControllerInfo("TestModuleId", 1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["controller"] = 1;
    params["component"] = "TestModuleId";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result["interface"].isString());
    ASSERT_TRUE(result["driveCount"].isUInt());
    ASSERT_TRUE(result["status"].isObject());
    ASSERT_TRUE(result["status"]["state"].isString());
    ASSERT_TRUE(result["status"]["health"].isString());
    ASSERT_TRUE(result["fruInfo"].isObject());
    ASSERT_TRUE(result["fruInfo"]["serialNumber"].isString());
    ASSERT_TRUE(result["fruInfo"]["manufacturer"].isString());
    ASSERT_TRUE(result["fruInfo"]["modelNumber"].isString());
    ASSERT_TRUE(result["fruInfo"]["partNumber"].isString());
    ASSERT_EQ(result["interface"].asString(), "Test Interface");
    ASSERT_EQ(result["driveCount"].asUInt(), 2);
    ASSERT_EQ(result["status"]["state"], "TestState");
    ASSERT_EQ(result["status"]["health"], "TestHealth");
    ASSERT_EQ(result["fruInfo"]["serialNumber"], "TestSerialNumber");
    ASSERT_EQ(result["fruInfo"]["manufacturer"], "TestManufacturer");
    ASSERT_EQ(result["fruInfo"]["modelNumber"], "TestModelNumber");
    ASSERT_EQ(result["fruInfo"]["partNumber"], "TestPartNumber");
}

TEST_F(GetStorageControllerInfoTest, NegativeModuleNotFound) {
    compute::json::GetStorageControllerInfo command_json;
    GetStorageControllerInfo* command = new GetStorageControllerInfo("TestModuleId", 1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["controller"] = 1;
    params["component"] = "OtherTestModuleId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

