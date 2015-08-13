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

#include "agent-framework/command/storage/add_logical_drive.hpp"
#include "json/add_logical_drive.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class AddLogicalDrive : public storage::AddLogicalDrive {
private:
    std::string m_type{};
    std::string m_mode{};
    std::string m_master{};
    bool m_snapshot{};
    bool m_protected{};
    std::vector<std::string> m_drives{};
public:
    AddLogicalDrive(
        std::string type,
        std::string mode,
        std::string master,
        bool snapshot,
        bool prot) {
        m_type = type;
        m_mode = mode;
        m_master = master;
        m_snapshot = snapshot;
        m_protected = prot;
        }

    using storage::AddLogicalDrive::execute;

    void execute(const Request& request, Response& response) {
        auto type = request.get_type();
        auto mode = request.get_mode();
        auto master = request.get_master();
        auto snapshot = request.is_snapshot();
        auto prot = request.is_protected();

        if (type != m_type
            || mode != m_mode
            || master != m_master
            || snapshot != m_snapshot
            || prot != m_protected) {
            throw exception::NotFound();
        }

        response.set_oem(agent_framework::generic::OEMData());
        response.set_drive_uuid("Test Drive UUID");
    }

    virtual ~AddLogicalDrive();
};

AddLogicalDrive::~AddLogicalDrive() { }

class AddLogicalDriveTest : public ::testing::Test {
protected:
    static constexpr char DRIVE[] = "drive";
    static constexpr char OEM[] = "oem";
    static constexpr char TYPE[] = "type";
    static constexpr char MODE[] = "mode";
    static constexpr char MASTER[] = "master";
    static constexpr char SNAPSHOT[] = "snapshot";
    static constexpr char PROTECTED[] = "protected";

    virtual ~AddLogicalDriveTest();
};

constexpr char AddLogicalDriveTest::DRIVE[];
constexpr char AddLogicalDriveTest::OEM[];
constexpr char AddLogicalDriveTest::TYPE[];
constexpr char AddLogicalDriveTest::MODE[];
constexpr char AddLogicalDriveTest::MASTER[];
constexpr char AddLogicalDriveTest::SNAPSHOT[];
constexpr char AddLogicalDriveTest::PROTECTED[];

AddLogicalDriveTest::~AddLogicalDriveTest() { }

TEST_F(AddLogicalDriveTest, PositiveExecute) {
    storage::json::AddLogicalDrive command_json;
    AddLogicalDrive* command = new AddLogicalDrive("TestType", "TestMode", "TestMaster", true, true);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TYPE] = "TestType";
    params[MODE] = "TestMode";
    params[MASTER] = "TestMaster";
    params[SNAPSHOT] = true;
    params[PROTECTED] = true;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_TRUE(result[DRIVE].isString());
    ASSERT_EQ(result[DRIVE].asString(), "Test Drive UUID");
}

TEST_F(AddLogicalDriveTest, NegativeTypeNotFound) {
    storage::json::AddLogicalDrive command_json;
    AddLogicalDrive* command = new AddLogicalDrive("TestType", "TestMode", "TestMaster", true, true);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TYPE] = "OtherTestType";
    params[MODE] = "TestMode";
    params[MASTER] = "TestMaster";
    params[SNAPSHOT] = true;
    params[PROTECTED] = true;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(AddLogicalDriveTest, NegativeModeNotFound) {
    storage::json::AddLogicalDrive command_json;
    AddLogicalDrive* command = new AddLogicalDrive("TestType", "TestMode", "TestMaster", true, true);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TYPE] = "TestType";
    params[MODE] = "OtherTestMode";
    params[MASTER] = "TestMaster";
    params[SNAPSHOT] = true;
    params[PROTECTED] = true;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(AddLogicalDriveTest, NegativeMasterNotFound) {
    storage::json::AddLogicalDrive command_json;
    AddLogicalDrive* command = new AddLogicalDrive("TestType", "TestMode", "TestMaster", true, true);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TYPE] = "TestType";
    params[MODE] = "TestMode";
    params[MASTER] = "OtherTestMaster";
    params[SNAPSHOT] = true;
    params[PROTECTED] = true;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(AddLogicalDriveTest, NegativeSnapshotNotFound) {
    storage::json::AddLogicalDrive command_json;
    AddLogicalDrive* command = new AddLogicalDrive("TestType", "TestMode", "TestMaster", true, true);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TYPE] = "TestType";
    params[MODE] = "TestMode";
    params[MASTER] = "TestMaster";
    params[SNAPSHOT] = false;
    params[PROTECTED] = true;

    EXPECT_ANY_THROW(command_json.method(params, result));
}

TEST_F(AddLogicalDriveTest, NegativeProtectedNotFound) {
    storage::json::AddLogicalDrive command_json;
    AddLogicalDrive* command = new AddLogicalDrive("TestType", "TestMode", "TestMaster", true, true);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TYPE] = "TestType";
    params[MODE] = "TestMode";
    params[MASTER] = "TestMaster";
    params[SNAPSHOT] = true;
    params[PROTECTED] = false;

    EXPECT_ANY_THROW(command_json.method(params, result));
}
