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
 * @file microhttpd.hpp
 *
 * @brief Declaration of HTTP server based on Libmicrohttpd
 * */

#ifndef PSME_REST_HTTP_MICROHTTPD_HPP
#define PSME_REST_HTTP_MICROHTTPD_HPP

#include "psme/rest/http/server.hpp"

#include <microhttpd.h>
#include <string>

namespace psme {
namespace rest {
namespace http {

using std::string;

/*!
 * @brief HTTP server implementation based on Libmicrohttpd.
 * */
class MicroHttpd : public Server {
public:
    MicroHttpd(const string& url);
    ~MicroHttpd();
    void open();
    void close();

private:
    struct MHD_Daemon* m_daemon;
    MicroHttpd(const MicroHttpd&) = delete;
    MicroHttpd& operator=(const MicroHttpd&) = delete;
    void start_daemon(uint16_t port, bool use_ssl);
};

}
}
}

#endif /* PSME_REST_HTTP_MICROHTTPD_HPP */
