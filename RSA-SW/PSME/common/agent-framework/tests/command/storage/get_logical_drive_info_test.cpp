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

#include "agent-framework/command/storage/get_logical_drive_info.hpp"
#include "json/get_logical_drive_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetLogicalDriveInfo : public storage::GetLogicalDriveInfo {
private:
    std::string m_drive{};

    class TestOEMData : public agent_framework::generic::OEMData {
    public:
        Json::Value to_json() const {
            Json::Value json_oem;
            json_oem["testOEM"] = "TestOEMSpecificData";
            return json_oem;
        }

        ~TestOEMData();
   };
public:
    GetLogicalDriveInfo(std::string drive) { m_drive = drive; }

    using storage::GetLogicalDriveInfo::execute;

    void execute(const Request& request, Response& response) {
        auto drive = request.get_drive();

        if (drive != m_drive) {
            throw exception::NotFound();
        }

        response.set_mode("Test Mode");
        response.set_type("Test Type");
        response.set_capacity_gb(2048);
        response.set_image("Test Image");
        response.set_bootable(true);
        response.set_protected(true);
        response.set_collection(agent_framework::generic::Collection());
        response.set_master("Test Master");
        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_oem(new TestOEMData);
    }

    virtual ~GetLogicalDriveInfo();
};

GetLogicalDriveInfo::~GetLogicalDriveInfo() { }

GetLogicalDriveInfo::TestOEMData::~TestOEMData() { }

class GetLogicalDriveInfoTest : public ::testing::Test {
protected:
    static constexpr char DRIVE[] = "drive";
    static constexpr char MODE[] = "mode";
    static constexpr char STATUS[] = "status";
    static constexpr char IMAGE[] = "image";
    static constexpr char TYPE[] = "type";
    static constexpr char CAPACITY_GB[] = "capacityGB";
    static constexpr char BOOTABLE[] = "bootable";
    static constexpr char OEM[] = "oem";
    static constexpr char PROTECTED[] = "protected";
    static constexpr char COLLECTIONS[] = "collections";
    static constexpr char MASTER[] = "master";
    static constexpr char STATE[] = "state";
    static constexpr char HEALTH[] = "health";

    virtual ~GetLogicalDriveInfoTest();
};

constexpr char GetLogicalDriveInfoTest::DRIVE[];
constexpr char GetLogicalDriveInfoTest::MODE[];
constexpr char GetLogicalDriveInfoTest::STATUS[];
constexpr char GetLogicalDriveInfoTest::IMAGE[];
constexpr char GetLogicalDriveInfoTest::TYPE[];
constexpr char GetLogicalDriveInfoTest::CAPACITY_GB[];
constexpr char GetLogicalDriveInfoTest::BOOTABLE[];
constexpr char GetLogicalDriveInfoTest::OEM[];
constexpr char GetLogicalDriveInfoTest::PROTECTED[];
constexpr char GetLogicalDriveInfoTest::COLLECTIONS[];
constexpr char GetLogicalDriveInfoTest::MASTER[];
constexpr char GetLogicalDriveInfoTest::STATE[];
constexpr char GetLogicalDriveInfoTest::HEALTH[];

GetLogicalDriveInfoTest::~GetLogicalDriveInfoTest() { }

TEST_F(GetLogicalDriveInfoTest, PositiveExecute) {
    storage::json::GetLogicalDriveInfo command_json;
    GetLogicalDriveInfo* command = new GetLogicalDriveInfo("TestDrive");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[DRIVE] = "TestDrive";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[IMAGE].isString());
    ASSERT_TRUE(result[TYPE].isString());
    ASSERT_TRUE(result[MODE].isString());
    ASSERT_TRUE(result[MASTER].isString());
    ASSERT_TRUE(result[PROTECTED].isBool());
    ASSERT_TRUE(result[CAPACITY_GB].isNumeric());
    ASSERT_TRUE(result[BOOTABLE].isBool());
    ASSERT_TRUE(result[COLLECTIONS].isArray());
    ASSERT_TRUE(result[STATUS].isObject());
    ASSERT_TRUE(result[STATUS][STATE].isString());
    ASSERT_TRUE(result[STATUS][HEALTH].isString());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_EQ(result[IMAGE].asString(), "Test Image");
    ASSERT_EQ(result[MODE].asString(), "Test Mode");
    ASSERT_EQ(result[MASTER].asString(), "Test Master");
    ASSERT_EQ(result[BOOTABLE].asBool(), true);
    ASSERT_EQ(result[PROTECTED].asBool(), true);
    ASSERT_EQ(result[TYPE].asString(), "Test Type");
    ASSERT_EQ(result[CAPACITY_GB].asInt(), 2048);
    ASSERT_EQ(result[STATUS][STATE], "TestState");
    ASSERT_EQ(result[STATUS][HEALTH], "TestHealth");
    ASSERT_EQ(result[OEM]["testOEM"], "TestOEMSpecificData");
}

TEST_F(GetLogicalDriveInfoTest, NegativeDriveNotFound) {
    storage::json::GetLogicalDriveInfo command_json;
    GetLogicalDriveInfo* command = new GetLogicalDriveInfo("TestDrive");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[DRIVE] = "OtherTestDrive";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
