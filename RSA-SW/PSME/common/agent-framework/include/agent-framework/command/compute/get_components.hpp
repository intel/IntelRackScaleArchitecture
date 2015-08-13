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
 * @file command/compute/get_components.hpp
 *
 * @brief Generic compute GetComponents command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COMPONENTS_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COMPONENTS_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/module.hpp"
#include "agent-framework/module/module_manager.hpp"

#include <string>
#include <vector>

/*! AGENT_FRAMEWORK namespace */
namespace agent_framework {
/*! Command namespace */
namespace command {
/*! Compute namespace */
namespace compute {

/* Forward declaration */
namespace json { class GetComponents; }

using std::string;
using module_vect_t = agent_framework::generic::ModuleManager::module_vec_t;

/*! Generic compute command GetComponents */
class GetComponents : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Compute";

    /*! Tag string for identify GetComponents command */
    static constexpr const char TAG[] = "getComponents";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetComponents();
protected:
    /*!
     * @brief Execute command with given command arguments
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
        friend class json::GetComponents;
        string m_component{};
    public:
        /*!
         * @brief Get component id from request
         * @return Component system uuid
         * */
        const string& get_component() const {
            return m_component;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:
        /*! Class representing Component attributes and subcomponents
         *  for GetComponents Response */
        class Component {
            string m_type{};
            string m_name{};
            std::vector<Component> m_components{};
            friend class json::GetComponents;
        public:
           /*!
            * @brief Component constructor
            * @param[in] type Component string like "RSAComputeModule"
            * @param[in] name Component string like "XeonComputeModule1"
            * @param[in] components Component subcomponents
            * */
            Component(string type = {}, string name = {}, std::vector<Component> components = {}) :
                m_type(type),
                m_name(name),
                m_components(components) {
            }

            /*!
            * @brief Component copy constructor
            * */
            Component(const Component&) = default;

           /*!
            * @brief Component assignment operator
            * */
            Component& operator=(const Component&) = default;

            /*!
            * @brief Set component type for response
            * @param[in] type Component string like "RSAComputeModule"
            * */
            inline void set_type(const string& type) {
                m_type = type;
            }
            /*!
             * @brief Set component name for response
             * @param[in] name Component string like "XeonComputeModule1"
             * */
            inline void set_name(const string& name) {
                m_name = name;
            }
            /*!
             * @brief Set components for response
             * @param[in] components Subcomponents vector like:
             *                              {"type" : "RsaBlade",
             *                               "name" : "Avoton1_3F602"}
             * */
            inline void set_components(std::vector<Component> components) {
                m_components = components;
            }

            ~Component();
        };

        /*! Default class constructor */
        Response(std::vector<Component> components = {}) : m_components(components) {}

        /*!
         * @brief Set components for response
         * @param[in] components Components vector
         * */
        inline void set_components(std::vector<Component> & components) {
            m_components = components;
        }
    private:
        std::vector<Component> m_components;
        friend class json::GetComponents;
    public:
        ~Response();
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COMPONENTS_HPP */
