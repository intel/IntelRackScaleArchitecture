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

#include "agent-framework/command/storage/get_target_info.hpp"
#include "json/get_target_info.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetTargetInfo : public storage::GetTargetInfo {
private:
    std::string m_target{};

    agent_framework::generic::Target create_demo_target() {
        agent_framework::generic::Target target;
        std::vector<agent_framework::generic::Target::Lun> vec_lun;
        vec_lun.push_back(agent_framework::generic::Target::Lun());
        target.set_target_iqn("Test Target IQN");
        target.set_target_lun(vec_lun);
        target.set_target_port(1);
        target.set_target_address("Test Target Address");
        target.set_oem_data(agent_framework::generic::OEMData());
        target.set_initiator_iqn("Test Initiator IQN");

        return target;
    }
public:
    GetTargetInfo(const std::string& target) { m_target = target; }

    using storage::GetTargetInfo::execute;

    void execute(const Request& request, Response& response) {
        auto target = request.get_target();

        if (target != m_target) {
            throw exception::NotFound();
        }

        response.set_target_data(create_demo_target());
    }

    virtual ~GetTargetInfo();
};

GetTargetInfo::~GetTargetInfo() { }

class GetTargetInfoTest : public ::testing::Test {
protected:
    static constexpr char TARGET[] = "target";
    static constexpr char TARGET_ADDRESS[] = "targetAddress";
    static constexpr char TARGET_PORT[] = "targetPort";
    static constexpr char TARGET_IQN[] = "targetIQN";
    static constexpr char TARGET_LUN[] = "targetLUN";
    static constexpr char INITIATOR_IQN[] = "initiatorIQN";
    static constexpr char OEM[] = "oem";

    virtual ~GetTargetInfoTest();
};

constexpr char GetTargetInfoTest::TARGET[];
constexpr char GetTargetInfoTest::TARGET_ADDRESS[];
constexpr char GetTargetInfoTest::TARGET_PORT[];
constexpr char GetTargetInfoTest::TARGET_IQN[];
constexpr char GetTargetInfoTest::TARGET_LUN[];
constexpr char GetTargetInfoTest::INITIATOR_IQN[];
constexpr char GetTargetInfoTest::OEM[];

GetTargetInfoTest::~GetTargetInfoTest() { }

TEST_F(GetTargetInfoTest, PositiveExecute) {
    storage::json::GetTargetInfo command_json;
    GetTargetInfo* command = new GetTargetInfo("TestTarget");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TARGET] = "TestTarget";

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_TRUE(result[TARGET_ADDRESS].isString());
    ASSERT_TRUE(result[TARGET_PORT].isUInt());
    ASSERT_TRUE(result[TARGET_IQN].isString());
    ASSERT_TRUE(result[TARGET_LUN].isArray());
    ASSERT_TRUE(result[INITIATOR_IQN].isString());
    ASSERT_TRUE(result[OEM].isObject());
    ASSERT_EQ(result[TARGET_ADDRESS].asString(), "Test Target Address");
    ASSERT_EQ(result[TARGET_PORT].asUInt(), 1);
    ASSERT_EQ(result[TARGET_IQN].asString(), "Test Target IQN");
    ASSERT_EQ(result[INITIATOR_IQN].asString(), "Test Initiator IQN");
}

TEST_F(GetTargetInfoTest, NegativeTargetNotFound) {
    storage::json::GetTargetInfo command_json;
    GetTargetInfo* command = new GetTargetInfo("TestTarget");

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[TARGET] = "OtherTestTarget";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
