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

#include "agent-framework/command/network/get_module_info.hpp"
#include "json/get_module_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetModuleInfo : public network::GetModuleInfo {
private:
    std::string m_component{};
public:
    GetModuleInfo(const std::string& component) { m_component = component; }

    using network::GetModuleInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();

        if (uuid != m_component) {
            throw exception::NotFound();
        }

        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_blade_count(1);
        response.set_switch_count(1);
        response.set_slot(1);
        response.set_fru_info(agent_framework::generic::FruInfo(
          "TestSerialNumber", "TestManufacturer", "TestModelNumber", "TestPartNumber"));
        response.set_oem(agent_framework::generic::OEMData());
    }

    virtual ~GetModuleInfo();
};

GetModuleInfo::~GetModuleInfo() { }

class GetModuleInfoTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char BLADE_COUNT[] = "bladeCount";
    static constexpr char SWITCH_COUNT[] = "switchCount";
    static constexpr char SLOT_NUMBER[] = "slot";
    static constexpr char STATUS[] = "status";
    static constexpr char STATE[] = "state";
    static constexpr char HEALTH[] = "health";
    static constexpr char FRU_INFO[] = "fruInfo";
    static constexpr char SERIAL_NUMBER[] = "serialNumber";
    static constexpr char MANUFACTURER[] = "manufacturer";
    static constexpr char MODEL_NUMBER[] = "modelNumber";
    static constexpr char PART_NUMBER[] = "partNumber";
    static constexpr char OEM[] = "oem";
    static constexpr char TEST_MODULE_ID[] = "TestModuleId";

    virtual ~GetModuleInfoTest();
};

constexpr char GetModuleInfoTest::COMPONENT[];
constexpr char GetModuleInfoTest::BLADE_COUNT[];
constexpr char GetModuleInfoTest::SWITCH_COUNT[];
constexpr char GetModuleInfoTest::SLOT_NUMBER[];
constexpr char GetModuleInfoTest::STATUS[];
constexpr char GetModuleInfoTest::STATE[];
constexpr char GetModuleInfoTest::HEALTH[];
constexpr char GetModuleInfoTest::FRU_INFO[];
constexpr char GetModuleInfoTest::SERIAL_NUMBER[];
constexpr char GetModuleInfoTest::MANUFACTURER[];
constexpr char GetModuleInfoTest::MODEL_NUMBER[];
constexpr char GetModuleInfoTest::PART_NUMBER[];
constexpr char GetModuleInfoTest::OEM[];
constexpr char GetModuleInfoTest::TEST_MODULE_ID[];

GetModuleInfoTest::~GetModuleInfoTest() { }

TEST_F(GetModuleInfoTest, PositiveExecute) {
    network::json::GetModuleInfo command_json;
    GetModuleInfo* command = new GetModuleInfo(TEST_MODULE_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_MODULE_ID;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[STATUS].isObject());
    ASSERT_TRUE(result[STATUS][STATE].isString());
    ASSERT_TRUE(result[STATUS][HEALTH].isString());
    ASSERT_TRUE(result[BLADE_COUNT].isUInt());
    ASSERT_TRUE(result[SWITCH_COUNT].isUInt());
    ASSERT_TRUE(result[SLOT_NUMBER].isUInt());
    ASSERT_TRUE(result[FRU_INFO].isObject());
    ASSERT_TRUE(result[FRU_INFO][SERIAL_NUMBER].isString());
    ASSERT_TRUE(result[FRU_INFO][MANUFACTURER].isString());
    ASSERT_TRUE(result[FRU_INFO][MODEL_NUMBER].isString());
    ASSERT_TRUE(result[FRU_INFO][PART_NUMBER].isString());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_EQ(result[STATUS][STATE], "TestState");
    ASSERT_EQ(result[STATUS][HEALTH], "TestHealth");
    ASSERT_EQ(result[BLADE_COUNT].asUInt(), 1);
    ASSERT_EQ(result[SWITCH_COUNT].asUInt(), 1);
    ASSERT_EQ(result[SLOT_NUMBER].asUInt(), 1);
    ASSERT_EQ(result[FRU_INFO][SERIAL_NUMBER], "TestSerialNumber");
    ASSERT_EQ(result[FRU_INFO][MANUFACTURER], "TestManufacturer");
    ASSERT_EQ(result[FRU_INFO][MODEL_NUMBER], "TestModelNumber");
    ASSERT_EQ(result[FRU_INFO][PART_NUMBER], "TestPartNumber");
}

TEST_F(GetModuleInfoTest, NegativeModuleNotFound) {
    network::json::GetModuleInfo command_json;
    GetModuleInfo* command = new GetModuleInfo(TEST_MODULE_ID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "OtherTestModuleId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
