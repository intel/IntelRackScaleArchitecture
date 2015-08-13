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

#include "agent-framework/command/compute/get_manager_info.hpp"
#include "json/get_manager_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetManagerInfo : public compute::GetManagerInfo {
private:
    std::string m_component{};
public:
    GetManagerInfo(const std::string& component) { m_component = component; }

    using compute::GetManagerInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();

        if (uuid != m_component) {
            throw exception::NotFound();
        }

        response.set_firmware_version("Test Firmware Version");
        response.set_ipv4_address("127.0.0.1");
        response.set_status_state("Test State");
        response.set_status_health("Test Health");
        response.set_serial_console_type("Test Signal Type");
        response.set_serial_console_bitrate(8);
        response.set_serial_console_parity("Test Parity");
        response.set_serial_console_databits(8);
        response.set_serial_console_stopbits(8);
        response.set_serial_console_flow_control("Test Flow Control");
        response.set_serial_console_pinout("Test Pin Out");
        response.set_serial_console_enabled(true);
    }

    virtual ~GetManagerInfo();
};

GetManagerInfo::~GetManagerInfo() { }

class GetManagerInfoTest : public ::testing::Test {
public:
    virtual ~GetManagerInfoTest();
};

GetManagerInfoTest::~GetManagerInfoTest() { }

TEST_F(GetManagerInfoTest, PositiveExecute) {
    compute::json::GetManagerInfo command_json;
    GetManagerInfo* command = new GetManagerInfo("TestModuleId");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "TestModuleId";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result["firmwareVersion"].isString());
    ASSERT_TRUE(result["ipv4address"].isString());
    ASSERT_TRUE(result["serialConsole"].isObject());
    ASSERT_TRUE(result["status"].isObject());
    ASSERT_TRUE(result["status"]["state"].isString());
    ASSERT_TRUE(result["status"]["health"].isString());
    ASSERT_TRUE(result["serialConsole"]["signalType"].isString());
    ASSERT_TRUE(result["serialConsole"]["bitrate"].isUInt());
    ASSERT_TRUE(result["serialConsole"]["parity"].isString());
    ASSERT_TRUE(result["serialConsole"]["dataBits"].isUInt());
    ASSERT_TRUE(result["serialConsole"]["stopBits"].isUInt());
    ASSERT_TRUE(result["serialConsole"]["flowControl"].isString());
    ASSERT_TRUE(result["serialConsole"]["pinOut"].isString());
    ASSERT_TRUE(result["serialConsole"]["enabled"].isBool());
    ASSERT_EQ(result["firmwareVersion"].asString(), "Test Firmware Version");
    ASSERT_EQ(result["ipv4address"].asString(), "127.0.0.1");
    ASSERT_EQ(result["status"]["state"].asString(), "Test State");
    ASSERT_EQ(result["status"]["health"].asString(), "Test Health");
    ASSERT_EQ(result["serialConsole"]["signalType"].asString(), "Test Signal Type");
    ASSERT_EQ(result["serialConsole"]["bitrate"].asUInt(), 8);
    ASSERT_EQ(result["serialConsole"]["parity"].asString(), "Test Parity");
    ASSERT_EQ(result["serialConsole"]["dataBits"].asUInt(), 8);
    ASSERT_EQ(result["serialConsole"]["stopBits"].asUInt(), 8);
    ASSERT_EQ(result["serialConsole"]["flowControl"].asString(), "Test Flow Control");
    ASSERT_EQ(result["serialConsole"]["pinOut"].asString(), "Test Pin Out");
    ASSERT_EQ(result["serialConsole"]["enabled"].asBool(), true);
}

TEST_F(GetManagerInfoTest, NegativeModuleNotFound) {
    compute::json::GetManagerInfo command_json;
    GetManagerInfo* command = new GetManagerInfo("TestModuleId");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "OtherTestModuleId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
