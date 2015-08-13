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

#include "agent-framework/command/compute/get_memory_info.hpp"
#include "json/get_memory_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetMemoryInfo : public compute::GetMemoryInfo {
private:
    std::string m_component{};
    unsigned int m_socket{};
public:
    GetMemoryInfo(
        const std::string& component,
        std::uint32_t socket) {
        m_component = component;
        m_socket = socket;
    }

    using compute::GetMemoryInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();
        auto socket = request.get_socket();

        if (uuid != m_component || socket != m_socket) {
            throw exception::NotFound();
        }

        response.set_socket("Test Socket 1");
        response.set_type("Test Type");
        response.set_size_gb(16);
        response.set_speed_mhz(1333);
        response.set_voltage_volt(3.2);
        response.set_data_width_bits(8);
        response.set_total_width_bits(10);
        response.set_form_factor("Test Form Factor");
    }

    virtual ~GetMemoryInfo();
};

GetMemoryInfo::~GetMemoryInfo() { }

class GetMemoryInfoTest : public ::testing::Test {
public:
    virtual ~GetMemoryInfoTest();
};

GetMemoryInfoTest::~GetMemoryInfoTest() { }

TEST_F(GetMemoryInfoTest, PositiveExecute) {
    compute::json::GetMemoryInfo command_json;
    GetMemoryInfo* command = new GetMemoryInfo("TestModuleId", 1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "TestModuleId";
    params["socket"] = 1;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result["socket"].isString());
    ASSERT_TRUE(result["type"].isString());
    ASSERT_TRUE(result["sizeGB"].isUInt());
    ASSERT_TRUE(result["speedMHz"].isUInt());
    ASSERT_TRUE(result["voltageVolt"].isNumeric());
    ASSERT_TRUE(result["dataWidthBits"].isUInt());
    ASSERT_TRUE(result["totalWidthBits"].isUInt());
    ASSERT_TRUE(result["formFactor"].isString());
    ASSERT_TRUE(result["fruInfo"].isObject());
    ASSERT_TRUE(result["status"].isObject());
    ASSERT_TRUE(result["oem"].isObject());
    ASSERT_EQ(result["socket"].asString(), "Test Socket 1");
    ASSERT_EQ(result["type"].asString(), "Test Type");
    ASSERT_EQ(result["sizeGB"].asUInt(), 16);
    ASSERT_EQ(result["speedMHz"].asUInt(), 1333);
    ASSERT_EQ(result["voltageVolt"].asDouble(), 3.2);
    ASSERT_EQ(result["dataWidthBits"].asUInt(), 8);
    ASSERT_EQ(result["totalWidthBits"].asUInt(), 10);
    ASSERT_EQ(result["formFactor"].asString(), "Test Form Factor");
}

TEST_F(GetMemoryInfoTest, NegativeModuleNotFound) {
    compute::json::GetMemoryInfo command_json;
    GetMemoryInfo* command = new GetMemoryInfo("TestModuleId", 1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "OtherTestModuleId";
    params["socket"] = 1;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetMemoryInfoTest, NegativeSlotNotFound) {
    compute::json::GetMemoryInfo command_json;
    GetMemoryInfo* command = new GetMemoryInfo("TestModuleId", 1);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "TestModuleId";
    params["socket"] = 2;

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
