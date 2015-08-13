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
 * @file psme_server_factory.hpp
 *
 * @brief Declaration of PSME Rest server factory
 * */
#ifndef PSME_REST_HTTP_PSME_SERVER_FACTORY_HPP
#define PSME_REST_HTTP_PSME_SERVER_FACTORY_HPP

#include <string>

namespace psme {
namespace rest {

namespace node {
    /*! Forward declaration */
    class TreeManager;
}

namespace http {

/*! Forward declaration */
class Server;

using psme::rest::node::TreeManager;

/*!
 * @brief PSME rest server factory.
 * */
class PsmeServerFactory {
public:
    /*!
     * @brief Singleton pattern. Get PSME server factory instance.
     *
     * @return Instance of PSME server factory
     * */
    static PsmeServerFactory& get_instance();

    /*!
     * @brief Create rest server instance.
     *
     * @param[in] url               Url on which server is listening
     * @param[in] tree_manager      Tree nodes manager
     *
     * @return Rest server instance
     * */
    Server* create_server(const std::string& url, TreeManager& tree_manager);

private:
    PsmeServerFactory(const PsmeServerFactory& orig) = delete;
    PsmeServerFactory& operator=(const PsmeServerFactory&) = delete;
    PsmeServerFactory() { }
    ~PsmeServerFactory() = delete;
};

}
}
}
#endif	/* PSME_REST_HTTP_PSME_SERVER_FACTORY_HPP */

