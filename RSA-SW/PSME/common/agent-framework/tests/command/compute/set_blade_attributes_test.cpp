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

#include "agent-framework/command/compute/set_blade_attributes.hpp"
#include "json/set_blade_attributes.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace agent_framework::command;
using namespace agent_framework::command::exception;

class TestBladeAttributes {
public:
    TestBladeAttributes() {}
    void set_component(const std::string& val) {
        m_component = val;
    }

    void set_boot_override(const std::string& val) {
        m_boot_override = val;
    }

    void set_boot_override_target(const std::string& val) {
        m_boot_override_target = val;
    }

    void set_power_state(const std::string& val) {
        m_power_state = val;
    }

    void set_oem(const agent_framework::generic::OEMData& val) {
        m_oem_data = val;
    }

    const std::string& get_component() const {
        return m_component;
    }

    const std::string& get_boot_override() const {
        return m_boot_override;
    }

    const std::string& get_boot_override_target() const {
        return m_boot_override_target;
    }

    const std::string& get_power_state() const {
        return m_power_state;
    }

    const agent_framework::generic::OEMData& get_oem() const {
        return m_oem_data;
    }

    virtual ~TestBladeAttributes();
private:
    std::string m_component;
    std::string m_boot_override;
    std::string m_boot_override_target;
    std::string m_power_state;
    agent_framework::generic::OEMData m_oem_data;
};

TestBladeAttributes::~TestBladeAttributes() { }

class SetBladeAttributes : public compute::SetBladeAttributes {
private:
    std::string m_component{};
    TestBladeAttributes* m_blade_attributes;
public:
    SetBladeAttributes(
        const std::string& component,
        TestBladeAttributes* blade_attributes) {
        m_component = component;
        m_blade_attributes = blade_attributes;
        }

    using compute::SetBladeAttributes::execute;

    void execute(const Request& request, Response&) {
        auto component = request.get_component();
        auto boot_override = request.get_boot_override();
        auto boot_override_target = request.get_boot_override_target();
        auto power_state = request.get_power_state();
        auto oem_data = request.get_oem();

        if (component != m_component ) {
                throw exception::NotFound();
        }

        m_blade_attributes->set_component(component);
        m_blade_attributes->set_boot_override(boot_override);
        m_blade_attributes->set_boot_override_target(boot_override_target);
        m_blade_attributes->set_power_state(power_state);
        m_blade_attributes->set_oem(oem_data);
    }

    virtual ~SetBladeAttributes();
};

SetBladeAttributes::~SetBladeAttributes() { }

class SetBladeAttributesTest : public ::testing::Test {
public:
    virtual ~SetBladeAttributesTest();
};

SetBladeAttributesTest::~SetBladeAttributesTest() { }

TEST_F(SetBladeAttributesTest, PositiveExecute) {
    compute::json::SetBladeAttributes command_json;
    TestBladeAttributes test_blade_attributes;
    SetBladeAttributes *command = new SetBladeAttributes("TestModuleId", &test_blade_attributes);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;
    agent_framework::generic::OEMData oem;
    params["component"] = "TestModuleId";
    params["bootOverride"] = "Disabled";
    params["bootOverrideTarget"] = "BootOverrideTarget";
    params["powerState"] = "PowerState";
    params["oem"] = oem.to_json();

    EXPECT_NO_THROW(command_json.method(params, result));

    ASSERT_TRUE(result.isObject());
    ASSERT_EQ(test_blade_attributes.get_boot_override(), "Disabled");
    ASSERT_EQ(test_blade_attributes.get_boot_override_target(), "BootOverrideTarget");
    ASSERT_EQ(test_blade_attributes.get_component(), "TestModuleId");
    ASSERT_EQ(test_blade_attributes.get_power_state(), "PowerState");
}

TEST_F(SetBladeAttributesTest, NegativeModuleNotFound) {
    compute::json::SetBladeAttributes command_json;
    TestBladeAttributes test_blade_attributes;
    SetBladeAttributes *command = new SetBladeAttributes("TestModuleId", &test_blade_attributes);

    EXPECT_NO_THROW(command_json.set_command(command));

    Json::Value params;
    Json::Value result;

    params["component"] = "OtherTestModuleId";

    EXPECT_THROW(command_json.method(params, result), jsonrpc::JsonRpcException);
}
