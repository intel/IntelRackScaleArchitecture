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
 * @file command/compute/get_collection.hpp
 * @brief Generic compute GetCollection command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COLLECTION_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COLLECTION_HPP

#include "agent-framework/command/command.hpp"

#include <vector>

namespace agent_framework {
namespace command {
namespace compute {

/* Forward declaration */
namespace json { class GetCollection; }

/*! Generic compute command GetCollection */
class GetCollection : public Command {
public:
    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Compute";

    /*! Tag string for identify GetCollection command */
    static constexpr const char TAG[] = "getCollection";

    /*! Argument request to execute */
    class Request : public Argument {
        friend class json::GetCollection;
        string m_component{};
        string m_name{};
    public:
        /*!
         * @brief Get component UUID
         * @return Component UUID
         * */
        const string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Get collection name
         * @return Collection name
         * */
        const string& get_name() const {
            return m_name;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:

        /*! Subcomponent (collection entry) */
        class Subcomponent {
            std::string m_subcomponent{};
            friend class json::GetCollection;
        public:
            /*! Default constructor */
            Subcomponent(std::string subcomponent = {})
                : m_subcomponent(subcomponent) {}

            /*! Copy constructor */
            Subcomponent(const Subcomponent&) = default;

            /*! Assignment operator */
            Subcomponent& operator=(const Subcomponent&) = default;

            /*! Destructor */
            ~Subcomponent();

            /*!
             * @brief Set subcomponent UUID
             * @param[in] subcomponent Subcomponent UUID
             * */
            void set_subcomponent(const std::string& subcomponent) {
                m_subcomponent = subcomponent;
            }
        };

        /*! Default class constructor */
        Response() {}

        /*!
         * @brief Add subcomponent to response
         * @param[in] subcomponent Subcomponent
         * */
        void add_subcomponent(const Subcomponent& subcomponent) {
            m_subcomponents.push_back(subcomponent);
        }
    private:
        std::vector<Subcomponent> m_subcomponents{};
        friend class json::GetCollection;
    public:
        ~Response();
    };

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetCollection();

protected:

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   in          Input request argument
     * @param[out]  out         Output response argument
     * */
    void execute(const Argument& in, Argument& out) override final {
        execute(static_cast<const Request&>(in), static_cast<Response&>(out));
    }
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COLLECTION_HPP */
