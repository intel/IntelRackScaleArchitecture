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
 * @file agent_unreachable.hpp
 *
 * @brief Agent Unreachable Exception.
 * */

#ifndef PSME_AGENT_UNREACHABLE_HPP
#define PSME_AGENT_UNREACHABLE_HPP

#include <string>
#include <exception>

namespace psme {
namespace core {
namespace agent {

/*!
 * @brief Agent Unreachable Exception.
 * */
class AgentUnreachable : public std::exception {
public:
    /*! @brief Constructor
     *  @param gami_id Unreachable agent's gami_id
     */
    explicit AgentUnreachable(const std::string& gami_id,
                              std::uint32_t unreachable_count,
                              std::uint32_t unreachable_seconds);

    /*! @brief Default copy constructor.  */
    AgentUnreachable(const AgentUnreachable&) = default;

    /*! @brief Assignment operator */
    AgentUnreachable& operator=(const AgentUnreachable&) = default;

    virtual ~AgentUnreachable() noexcept;

    /*!
     * @brief Gets Agent's gami_id.
     *
     * @return Gami_id of unreachable agent.
     * */
    const std::string& get_gami_id() const { return m_gami_id; }

    /*!
     * @brief Gets number of seconds agent is unreachable.
     *
     * @return Number of seconds agent is unreachable.
     * */
    std::uint32_t get_unreachable_seconds() const { return m_unreachable_seconds; }

    /*!
     * @brief Gets number of unreachable requests send to agent.
     *
     * @return Number of unreachable requests send to agent.
     * */
    std::uint32_t get_unreachable_count() const { return m_unreachable_count; }

    /*! Returns a C-style character string describing the general cause of
     *  the current error.  */
    virtual const char* what() const noexcept;

private:
    const std::string m_gami_id;
    std::uint32_t m_unreachable_count;
    std::uint32_t m_unreachable_seconds;
};

}
}
}
#endif /* PSME_AGENT_UNREACHABLE_HPP */
