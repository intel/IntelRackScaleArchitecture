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

#include "agent-framework/command/network/get_manager_info.hpp"
#include "json/get_manager_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetManagerInfo : public network::GetManagerInfo {
private:
    std::string m_component{};
public:
    GetManagerInfo(const std::string& component) { m_component = component; }

    using network::GetManagerInfo::execute;

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
    static constexpr char COMPONENT[] = "component";
    static constexpr char FIRMWARE_VERSION[] = "firmwareVersion";
    static constexpr char IPV4_ADDRESS[] = "ipv4address";
    static constexpr char STATUS[] = "status";
    static constexpr char SERIAL_CONSOLE[] = "serialConsole";
    static constexpr char TYPE[] = "signalType";
    static constexpr char BITRATE[] = "bitrate";
    static constexpr char PARITY[] = "parity";
    static constexpr char DATABITS[] = "dataBits";
    static constexpr char STOPBITS[] = "stopBits";
    static constexpr char FLOW_CONTROL[] = "flowControl";
    static constexpr char PINOUT[] = "pinOut";
    static constexpr char ENABLED[] = "enabled";
    static constexpr char STATE[] = "state";
    static constexpr char HEALTH[] = "health";

    virtual ~GetManagerInfoTest();
};

constexpr char GetManagerInfoTest::COMPONENT[];
constexpr char GetManagerInfoTest::FIRMWARE_VERSION[];
constexpr char GetManagerInfoTest::IPV4_ADDRESS[];
constexpr char GetManagerInfoTest::STATUS[];
constexpr char GetManagerInfoTest::SERIAL_CONSOLE[];
constexpr char GetManagerInfoTest::TYPE[];
constexpr char GetManagerInfoTest::BITRATE[];
constexpr char GetManagerInfoTest::PARITY[];
constexpr char GetManagerInfoTest::DATABITS[];
constexpr char GetManagerInfoTest::STOPBITS[];
constexpr char GetManagerInfoTest::FLOW_CONTROL[];
constexpr char GetManagerInfoTest::PINOUT[];
constexpr char GetManagerInfoTest::ENABLED[];
constexpr char GetManagerInfoTest::STATE[];
constexpr char GetManagerInfoTest::HEALTH[];

GetManagerInfoTest::~GetManagerInfoTest() { }

TEST_F(GetManagerInfoTest, PositiveExecute) {
    network::json::GetManagerInfo command_json;
    GetManagerInfo* command = new GetManagerInfo("TestModuleId");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestModuleId";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[FIRMWARE_VERSION].isString());
    ASSERT_TRUE(result[IPV4_ADDRESS].isString());
    ASSERT_TRUE(result[SERIAL_CONSOLE].isObject());
    ASSERT_TRUE(result[STATUS].isObject());
    ASSERT_TRUE(result[STATUS][STATE].isString());
    ASSERT_TRUE(result[STATUS][HEALTH].isString());
    ASSERT_TRUE(result[SERIAL_CONSOLE][TYPE].isString());
    ASSERT_TRUE(result[SERIAL_CONSOLE][BITRATE].isUInt());
    ASSERT_TRUE(result[SERIAL_CONSOLE][PARITY].isString());
    ASSERT_TRUE(result[SERIAL_CONSOLE][DATABITS].isUInt());
    ASSERT_TRUE(result[SERIAL_CONSOLE][STOPBITS].isUInt());
    ASSERT_TRUE(result[SERIAL_CONSOLE][FLOW_CONTROL].isString());
    ASSERT_TRUE(result[SERIAL_CONSOLE][PINOUT].isString());
    ASSERT_TRUE(result[SERIAL_CONSOLE][ENABLED].isBool());
    ASSERT_EQ(result[FIRMWARE_VERSION].asString(), "Test Firmware Version");
    ASSERT_EQ(result[IPV4_ADDRESS].asString(), "127.0.0.1");
    ASSERT_EQ(result[STATUS][STATE].asString(), "Test State");
    ASSERT_EQ(result[STATUS][HEALTH].asString(), "Test Health");
    ASSERT_EQ(result[SERIAL_CONSOLE][TYPE].asString(), "Test Signal Type");
    ASSERT_EQ(result[SERIAL_CONSOLE][BITRATE].asUInt(), 8);
    ASSERT_EQ(result[SERIAL_CONSOLE][PARITY].asString(), "Test Parity");
    ASSERT_EQ(result[SERIAL_CONSOLE][DATABITS].asUInt(), 8);
    ASSERT_EQ(result[SERIAL_CONSOLE][STOPBITS].asUInt(), 8);
    ASSERT_EQ(result[SERIAL_CONSOLE][FLOW_CONTROL].asString(), "Test Flow Control");
    ASSERT_EQ(result[SERIAL_CONSOLE][PINOUT].asString(), "Test Pin Out");
    ASSERT_EQ(result[SERIAL_CONSOLE][ENABLED].asBool(), true);
}

TEST_F(GetManagerInfoTest, NegativeModuleNotFound) {
    network::json::GetManagerInfo command_json;
    GetManagerInfo* command = new GetManagerInfo("TestModuleId");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "OtherTestModuleId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
