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

#include "agent-framework/command/compute/get_processor_info.hpp"
#include "json/get_processor_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetProcessorInfo : public compute::GetProcessorInfo {
private:
    std::string m_component{};
    unsigned int m_socket{};
public:
    GetProcessorInfo(
        const std::string& component,
        unsigned int socket) {
        m_component = component;
        m_socket = socket;
    }

    using compute::GetProcessorInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();
        auto socket = request.get_socket();

        if (uuid != m_component || socket != m_socket) {
            throw exception::NotFound();
        }

        response.set_processor_type("Test Processor Type");
        response.set_processor_architecture("Test Processor Architecture");
        response.set_instruction_set("Test Instruction Set");
        response.set_manufacturer("Test Manufacturer");
        response.set_model("Test Model");
        response.set_max_speed_mhz(2700);
        response.set_socket("CPU 0");
        response.set_total_cores(4);
        response.set_enabled_cores(2);
        response.set_total_threads(8);
        response.set_enabled_threads(6);
    }

    virtual ~GetProcessorInfo();
};

GetProcessorInfo::~GetProcessorInfo() { }

class GetProcessorInfoTest : public ::testing::Test {
public:
    virtual ~GetProcessorInfoTest();
};

GetProcessorInfoTest::~GetProcessorInfoTest() { }

TEST_F(GetProcessorInfoTest, PositiveExecute) {
    compute::json::GetProcessorInfo command_json;
    GetProcessorInfo* command = new GetProcessorInfo(
        "TestModuleId",
        1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "TestModuleId";
    params["socket"] = 1;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result["socket"].isString());
    ASSERT_TRUE(result["processorType"].isString());
    ASSERT_TRUE(result["processorArchitecture"].isString());
    ASSERT_TRUE(result["instructionSet"].isString());
    ASSERT_TRUE(result["manufacturer"].isString());
    ASSERT_TRUE(result["model"].isString());
    ASSERT_TRUE(result["maxSpeedMHz"].isNumeric());
    ASSERT_TRUE(result["totalCores"].isNumeric());
    ASSERT_TRUE(result["enabledCores"].isNumeric());
    ASSERT_TRUE(result["totalThreads"].isNumeric());
    ASSERT_TRUE(result["enabledThreads"].isNumeric());
    ASSERT_EQ(result["socket"].asString(), "CPU 0");
    ASSERT_EQ(result["processorType"].asString(), "Test Processor Type");
    ASSERT_EQ(result["processorArchitecture"].asString(), "Test Processor Architecture");
    ASSERT_EQ(result["instructionSet"].asString(), "Test Instruction Set");
    ASSERT_EQ(result["manufacturer"].asString(), "Test Manufacturer");
    ASSERT_EQ(result["model"].asString(), "Test Model");
    ASSERT_EQ(result["maxSpeedMHz"].asInt(), 2700);
    ASSERT_EQ(result["totalCores"].asInt(), 4);
    ASSERT_EQ(result["enabledCores"].asInt(), 2);
    ASSERT_EQ(result["totalThreads"].asInt(), 8);
    ASSERT_EQ(result["enabledThreads"].asInt(), 6);
}

TEST_F(GetProcessorInfoTest, NegativeModuleNotFound) {
    compute::json::GetProcessorInfo command_json;
    GetProcessorInfo* command = new GetProcessorInfo(
        "TestModuleId",
        1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "OtherTestModuleId";
    params["socket"] = 1;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetProcessorInfoTest, NegativeSlotNotFound) {
    compute::json::GetProcessorInfo command_json;
    GetProcessorInfo* command = new GetProcessorInfo(
        "TestModuleId",
        1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "TestModuleId";
    params["socket"] = 2;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
