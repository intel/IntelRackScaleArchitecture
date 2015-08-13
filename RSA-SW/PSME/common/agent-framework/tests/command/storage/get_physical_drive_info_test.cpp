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

#include "agent-framework/command/storage/get_physical_drive_info.hpp"
#include "json/get_physical_drive_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetPhysicalDriveInfo : public storage::GetPhysicalDriveInfo {
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
    GetPhysicalDriveInfo(std::string drive) { m_drive = drive; }

    using storage::GetPhysicalDriveInfo::execute;

    void execute(const Request& request, Response& response) {
        auto drive = request.get_drive();

        if (drive != m_drive) {
            throw exception::NotFound();
        }

        response.set_interface("Test Interface");
        response.set_type("Test Type");
        response.set_capacity_gb(2048);
        response.set_rpm(7200);
        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_fru_info(agent_framework::generic::FruInfo(
                                  "TestSerialNumber",
                                  "TestManufacturer",
                                  "TestModelNumber",
                                  "TestPartNumber"));
        response.set_oem(new TestOEMData);
    }

    virtual ~GetPhysicalDriveInfo();
};

GetPhysicalDriveInfo::~GetPhysicalDriveInfo() { }

GetPhysicalDriveInfo::TestOEMData::~TestOEMData() { }

class GetPhysicalDriveInfoTest : public ::testing::Test {
protected:
    static constexpr char DRIVE[] = "drive";
    static constexpr char FRU_INFO[] = "fruInfo";
    static constexpr char STATUS[] = "status";
    static constexpr char INTERFACE[] = "interface";
    static constexpr char TYPE[] = "type";
    static constexpr char CAPACITY_GB[] = "capacityGB";
    static constexpr char RPM[] = "rpm";
    static constexpr char OEM[] = "oem";
    static constexpr char SERIAL_NUMBER[] = "serialNumber";
    static constexpr char MANUFACTURER[] = "manufacturer";
    static constexpr char MODEL_NUMBER[] = "modelNumber";
    static constexpr char PART_NUMBER[] = "partNumber";
    static constexpr char STATE[] = "state";
    static constexpr char HEALTH[] = "health";

    virtual ~GetPhysicalDriveInfoTest();
};

constexpr char GetPhysicalDriveInfoTest::DRIVE[];
constexpr char GetPhysicalDriveInfoTest::FRU_INFO[];
constexpr char GetPhysicalDriveInfoTest::STATUS[];
constexpr char GetPhysicalDriveInfoTest::INTERFACE[];
constexpr char GetPhysicalDriveInfoTest::TYPE[];
constexpr char GetPhysicalDriveInfoTest::CAPACITY_GB[];
constexpr char GetPhysicalDriveInfoTest::RPM[];
constexpr char GetPhysicalDriveInfoTest::OEM[];
constexpr char GetPhysicalDriveInfoTest::SERIAL_NUMBER[];
constexpr char GetPhysicalDriveInfoTest::MANUFACTURER[];
constexpr char GetPhysicalDriveInfoTest::MODEL_NUMBER[];
constexpr char GetPhysicalDriveInfoTest::PART_NUMBER[];
constexpr char GetPhysicalDriveInfoTest::STATE[];
constexpr char GetPhysicalDriveInfoTest::HEALTH[];

GetPhysicalDriveInfoTest::~GetPhysicalDriveInfoTest() { }

TEST_F(GetPhysicalDriveInfoTest, PositiveExecute) {
    storage::json::GetPhysicalDriveInfo command_json;
    GetPhysicalDriveInfo* command = new GetPhysicalDriveInfo("TestDrive");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[DRIVE] = "TestDrive";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[INTERFACE].isString());
    ASSERT_TRUE(result[TYPE].isString());
    ASSERT_TRUE(result[CAPACITY_GB].isNumeric());
    ASSERT_TRUE(result[RPM].isNumeric());
    ASSERT_TRUE(result[STATUS].isObject());
    ASSERT_TRUE(result[STATUS][STATE].isString());
    ASSERT_TRUE(result[STATUS][HEALTH].isString());
    ASSERT_TRUE(result[FRU_INFO].isObject());
    ASSERT_TRUE(result[FRU_INFO][SERIAL_NUMBER].isString());
    ASSERT_TRUE(result[FRU_INFO][MANUFACTURER].isString());
    ASSERT_TRUE(result[FRU_INFO][MODEL_NUMBER].isString());
    ASSERT_TRUE(result[FRU_INFO][PART_NUMBER].isString());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_EQ(result[INTERFACE].asString(), "Test Interface");
    ASSERT_EQ(result[TYPE].asString(), "Test Type");
    ASSERT_EQ(result[CAPACITY_GB].asInt(), 2048);
    ASSERT_EQ(result[RPM].asInt(), 7200);
    ASSERT_EQ(result[STATUS][STATE], "TestState");
    ASSERT_EQ(result[STATUS][HEALTH], "TestHealth");
    ASSERT_EQ(result[FRU_INFO][SERIAL_NUMBER], "TestSerialNumber");
    ASSERT_EQ(result[FRU_INFO][MANUFACTURER], "TestManufacturer");
    ASSERT_EQ(result[FRU_INFO][MODEL_NUMBER], "TestModelNumber");
    ASSERT_EQ(result[FRU_INFO][PART_NUMBER], "TestPartNumber");
    ASSERT_EQ(result[OEM]["testOEM"], "TestOEMSpecificData");
}

TEST_F(GetPhysicalDriveInfoTest, NegativeDriveNotFound) {
    storage::json::GetPhysicalDriveInfo command_json;
    GetPhysicalDriveInfo* command = new GetPhysicalDriveInfo("TestDrive");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[DRIVE] = "OtherTestDrive";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
