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
 * @file blades.hpp
 *
 * @brief Declaration of Blade and Blades classes
 * */
#ifndef PSME_REST_NODE_BLADES_HPP
#define PSME_REST_NODE_BLADES_HPP

#include "psme/rest/node/node.hpp"

namespace psme {
namespace rest {
namespace node {

/*!
 * @brief Node representing Blade collection resource.
 * */
class Blades : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "Blades";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    Blades(const string& uuid = "",
           const string& gami_id = "",
           const string& id = TYPE);

    /*! @brief Destructor */
    ~Blades();

    /*!
     * @brief Tells if node represents resource collection.
     *
     * @return true if node represents resource collection, false otherwise.
     * */
    bool is_collection() const override { return true;}

    /*!
     * @brief Gets node type.
     *
     * @return Node's type.
     * */
    const char* get_type() const override { return TYPE;}
};

/*!
 * @brief Node representing Blade resource.
 * */
class Blade : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "Blade";
    /*! @brief JSON property */
    static constexpr const char PROP_NAME_BOOT[] = "Boot";
    /*! @brief JSON property */
    static constexpr const char
        PROP_NAME_BOOT_SOURCE_OVERRIDE_TARGET[] = "BootSourceOverrideTarget";
    /*! @brief JSON property */
    static constexpr const char
        PROP_NAME_BOOT_SOURCE_OVERRIDE_ENABLED[] = "BootSourceOverrideEnabled";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    Blade(const string& uuid = "",
          const string& gami_id = "",
          const string& id = "");

    /*! @brief Destructor */
    ~Blade();

    /*!
     * @brief Gets node type.
     *
     * @return Node's type.
     * */
    const char* get_type() const override { return TYPE;}

    /*!
     * @brief PATCH HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
    void patch(const Request& request, Response& response) override;

    class Actions : public Node {
    public:
        /*! @brief Type identifier */
        static constexpr const char TYPE[] = "Actions";

        /*!
         * @brief Constructor
         * @param[in] uuid Node's uuid known to agent.
         * @param[in] gami_id Node's agent uuid.
         * @param[in] id Node id in node's path.
         */
        Actions(const string& uuid = "",
                const string& gami_id = "",
                const string& id = TYPE);

        /*! @brief Destructor */
        ~Actions();

        /*!
         * @brief Gets node type.
         *
         * @return Node's type.
         * */
        const char* get_type() const override { return TYPE;}

        /*!
         * @brief GET HTTP method handler
         * @param[in] request HTTP request object
         * @param response HTTP response object
         */
        void get(const Request& request, Response& response) override {
            http_method_not_allowed(request, response);
        }

        class Reset : public Node {
        public:
            /*! @brief Type identifier */
            static constexpr const char TYPE[] = "RSABlade.Reset";
            /*! @brief JSON property */
            static constexpr const char ALLOWABLE_VALUES[]
                                            = "ResetType@DMTF.AllowableValues";
            /*! @brief JSON property */
            static constexpr const char PROP_NAME_RESET_TYPE[] = "ResetType";
            /*! @brief JSON property */
            static constexpr const char PROP_NAME_POWER[] = "Power";

            /*! @brief Type of reset (one of available values) */
            static constexpr const char RESET_TYPE_CONTINUOUS[] = "Continuous";

            /*!
             * @brief Constructor
             * @param[in] uuid Node's uuid known to agent.
             * @param[in] gami_id Node's agent uuid.
             * @param[in] id Node id in node's path.
             */
            Reset(const string& uuid = "",
                  const string& gami_id = "",
                  const string& id = TYPE);

            /*! @brief Destructor */
            ~Reset();

            /*!
             * @brief Gets node type.
             *
             * @return Node's type.
             * */
            const char* get_type() const override { return TYPE;}

            /*!
             * @brief POST HTTP method handler
             * @param[in] request HTTP request object
             * @param response HTTP response object
             */
            void post(const Request& request, Response& response) override;
        };
    };
};

}
}
}

#endif /* PSME_REST_NODE_BLADES_HPP */
