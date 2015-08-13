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
 *
 * @file capabilities.hpp
 *
 * @brief Agent capability
 * */

#ifndef PSME_AGENT_CAPABILITIES_HPP
#define PSME_AGENT_CAPABILITIES_HPP

#include <string>
#include <vector>

namespace psme {
namespace core {
/*! Register Agent */
namespace agent {
/*!
* @brief Capability attributes for agent
*/
class Capability {
public:
    /*!
    * @brief Create Capability object
    *
    * @param name agent name e.g. "compute"
    * @param version agent version
    */
    explicit Capability(const std::string& name, const std::string& version) :
            m_name{name},
            m_version{version} {}
    /*!
    * @brief Gets agent capability name
    *
    * @return name
    */
    const std::string& get_name() const {
        return m_name;
    }

    /*!
    * @brief Gets agent capability version
    *
    * @return version
    */
    const std::string& get_version() const {
        return m_version;
    }

    bool operator==(const Capability& c) const {
        return m_name == c.m_name
               && m_version == c.m_version;
    }

private:
    std::string m_name{};
    std::string m_version{};
};

/*! Capabilitis array */
using Capabilities = std::vector<Capability>;

}
}
}
#endif /* PSME_AGENT_CAPABILITIES_HPP */
