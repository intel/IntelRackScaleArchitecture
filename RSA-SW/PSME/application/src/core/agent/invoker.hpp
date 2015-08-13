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
 * @file invoker.hpp
 *
 * @brief Invoker abstract interface.
 * */

#ifndef PSME_INVOKER_HPP
#define PSME_INVOKER_HPP

#include <string>

namespace psme {
namespace core {

/*! Forward declaration RequestDTO and ResponseDTO class */
namespace dto {
class RequestDTO;
class ResponseDTO;
}

namespace agent {

/*! Invoker interface */
class Invoker {
public:
    /*! @brief ConnectionStatus
     *
     * First element contains number of failed requests send to agent,
     * second element is number of seconds agent is unavailable.
     */
    using ConnectionStatus = std::pair<std::uint32_t, std::uint32_t>;

    /*!
     * @brief Destroy Invoker
     */
    virtual ~Invoker();

    /*!
     * @brief Execute command with request, response agruments
     *
     * @param command Command name to execute
     * @param request Request object to transfer
     * @param response Response object to transfer
     */
    virtual void execute(const std::string& command,
                        psme::core::dto::RequestDTO& request,
                        psme::core::dto::ResponseDTO& response) = 0;

    /*!
     * @brief Gets connection status
     *
     * ConnectionStatus is a pair.
     * First element contains number of failed requests send to agent,
     * second element is number of seconds agent is unavailable.
     *
     * @return ConnectionStatus
     */
    virtual ConnectionStatus get_connection_status() const { return {0,0};}
};
}
}
}
#endif /* PSME_INVOKER_HPP */
