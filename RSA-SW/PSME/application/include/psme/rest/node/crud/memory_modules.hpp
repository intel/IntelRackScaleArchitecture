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
 * @file memory_modules.hpp
 *
 * @brief Declaration of MemoryModule and MemoryModules classes
 * */
#ifndef PSME_REST_NODE_MEMORY_HPP
#define PSME_REST_NODE_MEMORY_HPP

#include "psme/rest/node/node.hpp"

namespace psme {
namespace rest {
namespace node {

/*!
 * @brief Node representing Memory collection resource.
 * */
class MemoryModules : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "Memory";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    MemoryModules(const string& uuid = "",
                  const string& gami_id = "",
                  const string& id = TYPE);

    /*! @brief Destructor */
    ~MemoryModules();

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
 * @brief Node representing Memory resource.
 * */
class MemoryModule : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "MemoryModule";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    MemoryModule(const string& uuid = "",
                 const string& gami_id = "",
                 const string& id = "");

    /*! @brief Destructor */
    ~MemoryModule();

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

#endif /* PSME_REST_NODE_MEMORY_HPP */
