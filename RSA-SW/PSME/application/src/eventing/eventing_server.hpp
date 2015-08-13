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
 * */
#include "eventing_http_server.hpp"
#include "command_json_server.hpp"
#include "logger/logger_factory.hpp"

#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>

#include <memory>
#include <string>
#include <functional>
#include <thread>
#include <atomic>

namespace json {
    /*! Forward declaration */
    class Value;
}

/*! PSME namespace */
namespace psme {
/*! Application namespace */
namespace app {
/*! Eventing namespace */
namespace eventing {

/*! EventingServer implementation */
class EventingServer {
public:

    /*!
     * @brief EventingServer constructor
     *
     * @param config Configuration JSON object
     */
    explicit EventingServer(const ::json::Value& config);

    /*! EventingServer destructor */
    ~EventingServer();

    /*! Start server */
    void start();

    /*! Stop server */
    void stop();

    /*! Add commands to server */
    void add(const psme::command::CommandJson::Map::command_map_t& command_map);

private:
    const ::json::Value& m_config;
    EventingHttpServer m_http_server;
    psme::command::CommandJsonServer  m_command_json_server;

    std::thread m_thread{};
    std::atomic<bool> m_running{false};
};

using EventingServerSharedPtr = std::shared_ptr<EventingServer>;
using EventingServerUniquePtr = std::unique_ptr<EventingServer>;

}
}
}
