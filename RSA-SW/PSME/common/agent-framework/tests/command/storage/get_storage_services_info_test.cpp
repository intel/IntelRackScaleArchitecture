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

#include "agent-framework/command/storage/get_storage_services_info.hpp"
#include "json/get_storage_services_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetStorageServicesInfo : public storage::GetStorageServicesInfo {
private:
    std::string m_services{};

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
    GetStorageServicesInfo(std::string services) { m_services = services; }

    using storage::GetStorageServicesInfo::execute;

    void execute(const Request& request, Response& response) {
        auto services = request.get_services();

        if (services != m_services) {
            throw exception::NotFound();
        }

        response.add_collection(agent_framework::generic::Collection::CollectionEntry("TestName", "TestType", "TestSlotMask"));
        response.set_status(agent_framework::generic::Status("TestState", "TestHealth"));
        response.set_oem(new TestOEMData);
    }

    virtual ~GetStorageServicesInfo();
};

GetStorageServicesInfo::~GetStorageServicesInfo() { }

GetStorageServicesInfo::TestOEMData::~TestOEMData() { }

class GetStorageServicesInfoTest : public ::testing::Test {
protected:
    static constexpr char SERVICES[] = "services";
    static constexpr char COLLECTIONS[] = "collections";
    static constexpr char STATUS[] = "status";
    static constexpr char OEM[] = "oem";
    static constexpr char STATE[] = "state";
    static constexpr char HEALTH[] = "health";

    virtual ~GetStorageServicesInfoTest();
};

constexpr char GetStorageServicesInfoTest::SERVICES[];
constexpr char GetStorageServicesInfoTest::COLLECTIONS[];
constexpr char GetStorageServicesInfoTest::STATUS[];
constexpr char GetStorageServicesInfoTest::OEM[];
constexpr char GetStorageServicesInfoTest::STATE[];
constexpr char GetStorageServicesInfoTest::HEALTH[];

GetStorageServicesInfoTest::~GetStorageServicesInfoTest() { }

TEST_F(GetStorageServicesInfoTest, PositiveExecute) {
    storage::json::GetStorageServicesInfo command_json;
    GetStorageServicesInfo* command = new GetStorageServicesInfo("TestServices");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[SERVICES] = "TestServices";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[COLLECTIONS].isArray());
    ASSERT_TRUE(result[STATUS].isObject());
    ASSERT_TRUE(result[STATUS][STATE].isString());
    ASSERT_TRUE(result[STATUS][HEALTH].isString());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_EQ(result[STATUS][STATE], "TestState");
    ASSERT_EQ(result[STATUS][HEALTH], "TestHealth");
    ASSERT_EQ(result[OEM]["testOEM"], "TestOEMSpecificData");
}

TEST_F(GetStorageServicesInfoTest, NegativeServicesNotFound) {
    storage::json::GetStorageServicesInfo command_json;
    GetStorageServicesInfo* command = new GetStorageServicesInfo("TestServices");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[SERVICES] = "OtherTestServices";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
