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

#include "agent-framework/command/storage/get_component_collection.hpp"
#include "json/get_component_collection.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class GetComponentCollection : public storage::GetComponentCollection {
public:
    GetComponentCollection() { }

    using storage::GetComponentCollection::execute;

    void execute(const Request&, Response& response) {

        for (std::uint32_t i = 0; i < 10; ++i) {
            response.add_component(Response::Component(std::to_string(i), "Type"));
        }
    }

    virtual ~GetComponentCollection();
};

GetComponentCollection::~GetComponentCollection() { }

class GetComponentCollectionTest : public ::testing::Test {
public:
    virtual ~GetComponentCollectionTest();
};

GetComponentCollectionTest::~GetComponentCollectionTest() { }

TEST_F(GetComponentCollectionTest, PositiveExecute) {
    storage::json::GetComponentCollection command_json;
    GetComponentCollection* command = new GetComponentCollection();

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isArray());
    ASSERT_EQ(result.size(), 10);
    for (std::uint32_t i = 0; i < 10; ++i) {
        ASSERT_EQ(result[i]["component"].asString(), std::to_string(i));
    }
}
