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
 * @file command/storage/get_component_collection.hpp
 * @brief Generic storage GetComponentCollection command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_STORAGE_GET_COMPONENT_COLLECTION_HPP
#define AGENT_FRAMEWORK_COMMAND_STORAGE_GET_COMPONENT_COLLECTION_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/module_manager.hpp"

#include <string>
#include <vector>

namespace agent_framework {
namespace command {
namespace storage {

namespace json {
    /* Forward declaration */
    class GetComponentCollection;
}

class GetComponentCollection : public Command {
protected:
    void execute(const Argument& in, Argument& out) override final {
        execute(static_cast<const Request&>(in), static_cast<Response&>(out));
    }

public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Storage";

    /*! Tag string for identify GetComponentCollection command */
    static constexpr const char TAG[] = "getComponentCollection";

    /*!
     * @brief Executes request command and fills response argument;
     *
     * @param[in]  request    Input request argument
     * @param[out] response   Output response result
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    virtual ~GetComponentCollection();

    /*! Request representation for GetComponentCollection call */
    class Request : public Argument {
    public:
        ~Request();
    };

    /*! Response representation for GetComponentCollection call */
    class Response : public Argument {
    public:

        /*! Component for GetComponentCollection response */
        class Component {
            std::string m_component;
            std::string m_type;
            friend class json::GetComponentCollection;
        public:
            /*!
             * @brief Default constructor
             * @param[in] component Component UUID
             * @param[in] type Component type
             * */
            Component(const std::string& component = {},
                      const std::string& type = {})
                : m_component(component), m_type(type) {}

            /*! Copy constructor */
            Component(const Component&) = default;

            /*! Assignment operator */
            Component& operator=(const Component&) = default;

            /*! Destructor */
            ~Component();

            /*!
             * @brief Set component UUID
             * @param[in] component Component UUID
             * */
            void set_component(const std::string& component) {
                m_component = component;
            }

            /*!
             * @brief Set component type
             * @param[in] type Component type
             * */
            void set_type(const std::string& type) {
                m_type = type;
            }

        };

        /*! Default constructor */
        Response() {}

        /*! Destructor */
        ~Response();

        void add_component(const Component& component) {
            m_components.push_back(component);
        }

    private:
        std::vector<Component> m_components{};
        friend class json::GetComponentCollection;
    };
};

}/*! Storage namespace */
}/*! Command namespace */
}/*! AGENT_FRAMEWORK namespace */

#endif /* AGENT_FRAMEWORK_COMMAND_STORAGE_GET_COMPONENT_COLLECTION_HPP */
