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

#include "agent-framework/command/network/get_switch_info.hpp"
#include "json/get_switch_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetSwitchInfo : public network::GetSwitchInfo {
private:
    std::string m_component{};
public:
    GetSwitchInfo(const std::string& component) { m_component = component; }

    using network::GetSwitchInfo::execute;

    void execute(const Request& request, Response& response) {
        auto component = request.get_component();

        if (component != m_component) {
            throw exception::NotFound();
        }

        response.set_technology("Test Technology");
        response.set_mac_address("Test Mac Address");
        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_fru_info(agent_framework::generic::FruInfo(
                                  "TestSerialNumber",
                                  "TestManufacturer",
                                  "TestModelNumber",
                                  "TestPartNumber"));
        response.set_oem(agent_framework::generic::OEMData());
    }

    virtual ~GetSwitchInfo();
};

GetSwitchInfo::~GetSwitchInfo() { }

class GetSwitchInfoTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char TECHNOLOGY[] = "technology";
    static constexpr char MAC_ADDRESS[] = "macAddress";
    static constexpr char STATUS[] = "status";
    static constexpr char STATE[] = "state";
    static constexpr char HEALTH[] = "health";
    static constexpr char FRU_INFO[] = "fruInfo";
    static constexpr char SERIAL_NUMBER[] = "serialNumber";
    static constexpr char MANUFACTURER[] = "manufacturer";
    static constexpr char MODEL_NUMBER[] = "modelNumber";
    static constexpr char PART_NUMBER[] = "partNumber";
    static constexpr char OEM[] = "oem";

    virtual ~GetSwitchInfoTest();
};

constexpr char GetSwitchInfoTest::COMPONENT[];
constexpr char GetSwitchInfoTest::TECHNOLOGY[];
constexpr char GetSwitchInfoTest::MAC_ADDRESS[];
constexpr char GetSwitchInfoTest::STATUS[];
constexpr char GetSwitchInfoTest::STATE[];
constexpr char GetSwitchInfoTest::HEALTH[];
constexpr char GetSwitchInfoTest::FRU_INFO[];
constexpr char GetSwitchInfoTest::SERIAL_NUMBER[];
constexpr char GetSwitchInfoTest::MANUFACTURER[];
constexpr char GetSwitchInfoTest::MODEL_NUMBER[];
constexpr char GetSwitchInfoTest::PART_NUMBER[];
constexpr char GetSwitchInfoTest::OEM[];

GetSwitchInfoTest::~GetSwitchInfoTest() { }

TEST_F(GetSwitchInfoTest, PositiveExecute) {
    network::json::GetSwitchInfo command_json;
    GetSwitchInfo* command = new GetSwitchInfo("8d2c1ac0-2f82-11e5-8333-0002a5d5c51b");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51b";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[TECHNOLOGY].isString());
    ASSERT_TRUE(result[MAC_ADDRESS].isString());
    ASSERT_TRUE(result[STATUS].isObject());
    ASSERT_TRUE(result[STATUS][STATE].isString());
    ASSERT_TRUE(result[STATUS][HEALTH].isString());
    ASSERT_TRUE(result[FRU_INFO].isObject());
    ASSERT_TRUE(result[FRU_INFO][SERIAL_NUMBER].isString());
    ASSERT_TRUE(result[FRU_INFO][MANUFACTURER].isString());
    ASSERT_TRUE(result[FRU_INFO][MODEL_NUMBER].isString());
    ASSERT_TRUE(result[FRU_INFO][PART_NUMBER].isString());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_EQ(result[TECHNOLOGY].asString(), "Test Technology");
    ASSERT_EQ(result[MAC_ADDRESS].asString(), "Test Mac Address");
    ASSERT_EQ(result[STATUS][STATE], "TestState");
    ASSERT_EQ(result[STATUS][HEALTH], "TestHealth");
    ASSERT_EQ(result[FRU_INFO][SERIAL_NUMBER], "TestSerialNumber");
    ASSERT_EQ(result[FRU_INFO][MANUFACTURER], "TestManufacturer");
    ASSERT_EQ(result[FRU_INFO][MODEL_NUMBER], "TestModelNumber");
    ASSERT_EQ(result[FRU_INFO][PART_NUMBER], "TestPartNumber");
}

TEST_F(GetSwitchInfoTest, NegativeComponentNotFound) {
    network::json::GetSwitchInfo command_json;
    GetSwitchInfo* command = new GetSwitchInfo("8d2c1ac0-2f82-11e5-8333-0002a5d5c51b");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "8d2c1ac0-2f82-11e5-8333-0002a5d5c51c";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}

TEST_F(GetSwitchInfoTest, NegativeInvalidUUIDFormat) {
    network::json::GetSwitchInfo command_json;
    GetSwitchInfo* command = new GetSwitchInfo("8d2c1ac0-2f82-11e5-8333-0002a5d5c51b");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "TestUUID";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
