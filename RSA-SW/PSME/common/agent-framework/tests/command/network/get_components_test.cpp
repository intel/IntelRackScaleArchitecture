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

#include "agent-framework/command/network/get_components.hpp"
#include "json/get_components.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetComponents : public network::GetComponents {
private:
    std::string m_uuid{};
public:
    GetComponents(const std::string& uuid) { m_uuid = uuid; }

    using network::GetComponents::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();

        if (uuid != m_uuid) {
            throw exception::NotFound();
        }

        std::vector<Response::Component> vec_components{};

        for (std::uint32_t i = 0; i < 10; ++i) {
            vec_components.push_back(Response::Component("Type", std::to_string(i)));
        }

        response.set_components(vec_components);
    }

    virtual ~GetComponents();
};

GetComponents::~GetComponents() { }

class GetComponentsTest : public ::testing::Test {
public:
    static constexpr char COMPONENT[] = "component";
    static constexpr char TEST_UUID[] = "TestUUID";

    virtual ~GetComponentsTest();
};

constexpr char GetComponentsTest::COMPONENT[];
constexpr char GetComponentsTest::TEST_UUID[];

GetComponentsTest::~GetComponentsTest() { }

TEST_F(GetComponentsTest, PositiveExecute) {
    network::json::GetComponents command_json;
    GetComponents* command = new GetComponents(TEST_UUID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = TEST_UUID;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isArray());
    ASSERT_EQ(result.size(), 10);
    for (std::uint32_t i = 0; i < 10; ++i) {
        ASSERT_EQ(result[i][COMPONENT].asString(), std::to_string(i));
    }
}

TEST_F(GetComponentsTest, NegativeModuleNotFound) {
    network::json::GetComponents command_json;
    GetComponents* command = new GetComponents(TEST_UUID);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params[COMPONENT] = "OtherTestUUID";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
