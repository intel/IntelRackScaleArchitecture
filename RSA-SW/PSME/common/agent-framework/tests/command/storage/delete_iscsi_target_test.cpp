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

#include "agent-framework/command/storage/delete_iscsi_target.hpp"
#include "json/delete_iscsi_target.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class DeleteISCSITarget : public storage::DeleteISCSITarget {
private:
    std::string m_target{};
public:
    DeleteISCSITarget(std::string target) { m_target = target; }

    using storage::DeleteISCSITarget::execute;

    void execute(const Request& request, Response& response) {
        auto target = request.get_target();

        if (target != m_target) {
            throw exception::NotFound();
        }

        response.set_oem(agent_framework::generic::OEMData());
    }

    virtual ~DeleteISCSITarget();
};

DeleteISCSITarget::~DeleteISCSITarget() { }

class DeleteISCSITargetTest : public ::testing::Test {
protected:
    static constexpr char TARGET[] = "target";
    static constexpr char OEM[] = "oem";

    virtual ~DeleteISCSITargetTest();
};

constexpr char DeleteISCSITargetTest::TARGET[];
constexpr char DeleteISCSITargetTest::OEM[];

DeleteISCSITargetTest::~DeleteISCSITargetTest() { }

TEST_F(DeleteISCSITargetTest, PositiveExecute) {
    storage::json::DeleteISCSITarget command_json;
    DeleteISCSITarget* command = new DeleteISCSITarget("TestTarget");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TARGET] = "TestTarget";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[OEM].isObject());
}

TEST_F(DeleteISCSITargetTest, NegativeTargetNotFound) {
    storage::json::DeleteISCSITarget command_json;
    DeleteISCSITarget* command = new DeleteISCSITarget("TestTarget");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TARGET] = "OtherTestTarget";

    EXPECT_ANY_THROW(command_json.method(params, result));
}
