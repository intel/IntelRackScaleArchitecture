/*!
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
 *
 * @file command/compute/set_blade_attributes.hpp
 * @brief Generic compute SetBladeAttributes command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_SET_BLADE_ATTRIBUTES_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_SET_BLADE_ATTRIBUTES_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/oem_data.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace compute {

using OEMData = agent_framework::generic::OEMData;

/* Forward declaration */
namespace json { class SetBladeAttributes; }

/*! Generic compute command SetBladeAttributes */
class SetBladeAttributes : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Compute";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "setBladeAttributes";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~SetBladeAttributes();
protected:
    /*!
     * @brief Execute command with givent command arguments
     *
     * @param[in]   in      Input command argument
     * @param[out]  out     Output command argument
     * */
    void execute(const Argument& in, Argument& out) override final {
        execute(static_cast<const Request&>(in), static_cast<Response&>(out));
    }
public:
    /*! Argument request to execute */
    class Request : public Argument {
    private:
        friend class json::SetBladeAttributes;
        std::string m_component{};
        std::string m_boot_override{};
        std::string m_boot_override_target{};
        std::string m_power_state{};
        OEMData m_oem_data{};
    public:
        /*!
         * @brief Get component from request
         * @return uuid string
         * */
        const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get boot overrride
         * @return Boot override value
         */
        const std::string& get_boot_override() const {
            return m_boot_override;
        }

        /*!
         * @brief Get boot override target
         * @return Boot override target value
         */
        const std::string& get_boot_override_target() const {
            return m_boot_override_target;
        }

        /*!
         * @brief Get power state of the Blade in request.
         * @return string contains power state.
         */
        const std::string& get_power_state() const {
            return m_power_state;
        }

        /*!
         * @brief Get oem data
         * @return OEMData object
         */
        const OEMData& get_oem() const {
            return m_oem_data;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    private:
        friend class json::SetBladeAttributes;
    public:

        ~Response();
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_SET_BLADE_ATTRIBUTES_HPP */
