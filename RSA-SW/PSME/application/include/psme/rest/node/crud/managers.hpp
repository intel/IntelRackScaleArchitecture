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
 * @file managers.hpp
 *
 * @brief Declaration of Manager and Managers classes
 * */
#ifndef PSME_REST_NODE_MANAGERS_HPP
#define PSME_REST_NODE_MANAGERS_HPP

#include "psme/rest/node/node.hpp"

/* Forward declaration */
namespace json { class Value; }

namespace psme {
namespace rest {
namespace node {

/*!
 * @brief Node representing Manager collection resource.
 * */
class Managers : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "Managers";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    Managers(const string& uuid = "",
             const string& gami_id = "",
             const string& id = TYPE);

    /*! @brief Destructor */
    ~Managers();

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

    /*!
     * @brief Swaps odata_context.
     *
     * @param config Application's config
     * */
    void set_odata_context(const json::Value& config);
};

/*!
 * @brief Node representing Manager resource.
 * */
class Manager : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "Manager";
    /*! @brief Link name */
    static constexpr const char MANAGER_FOR_DRAWERS[] = "ManagerForChassis";
    /*! @brief Link name */
    static constexpr const char MANAGER_FOR_COMPUTEMODULES[] =
                                                  "ManagerForComputeModules";
    /*! @brief Link name */
    static constexpr const char MANAGER_FOR_FABRICMODULES[] =
                                                  "ManagerForFabricModules";
    /*! @brief Link name */
    static constexpr const char MANAGER_FOR_BLADES[] = "ManagerForBlades";
    /*! @brief Link name */
    static constexpr const char MANAGER_FOR_SWITCHES[] = "ManagerForSwitches";
    /*! @brief Link name */
    static constexpr const char SIMPLE_NETWORK[] = "SimpleNetwork";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    Manager(const string& uuid = "",
            const string& gami_id = "",
            const string& id = "");

    /*! @brief Destructor */
    ~Manager();

    /*!
     * @brief Gets node type.
     *
     * @return Node's type.
     * */
    const char* get_type() const override { return TYPE;}
};

}
}
}

#endif /* PSME_REST_NODE_MANAGERS_HPP */
