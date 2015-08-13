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
*/

#ifndef PSME_HTTPSERVERCONNECTOR_HPP
#define PSME_HTTPSERVERCONNECTOR_HPP

#include <sstream>
#include <map>
#include <microhttpd.h>
#include <jsonrpccpp/server/abstractserverconnector.h>
#include <jsonrpccpp/client/iclientconnector.h>

struct mhd_coninfo;

namespace json {
    /*! Forward declaration */
    class Value;
}

namespace psme {
namespace app {
namespace eventing {

using namespace jsonrpc;

/*!
* This class provides an embedded HTTP Server, based on libmicrohttpd,
* to handle incoming Requests and send HTTP 1.1
* valid responses.
* Note that this class will always send HTTP-Status 200, even though
* an JSON-RPC Error might have occurred. Please
* always check for the JSON-RPC Error Header.
*/
class EventingHttpServer: public AbstractServerConnector
{
public:

    /*!
    * @brief EventingHttpServer, constructor
    * for the included EventingHttpServer
    * @param config Configuration JSON object.
    */
    EventingHttpServer(const json::Value& config);

    /*! Delete copy constructor */
    EventingHttpServer(const EventingHttpServer&) = delete;

    /*! Delete assigment operator */
    EventingHttpServer& operator=(const EventingHttpServer&) = delete;

    /*!
     * @brief Start eventing http server
     *
     * @return On success true is returned, false otherwise
     */
    virtual bool StartListening();

    /*!
     * @brief Stop eventing http server
     *
     * @return On success true is returned, false otherwise
     */
    virtual bool StopListening();


    /*!
     * @brief Send response to client
     *
     * @param response response msg JSON format
     * @param addInfo pointer to additional information to send
     *
     * @return On success true is returned, false otherwise
     */
    bool virtual SendResponse(const std::string& response,
    void* addInfo = nullptr);


    /*!
     * @brief Send additional information to client
     *
     * @param addInfo pointer do additional data
     *
     * @return On success true is returned, false otherwise
     */
    bool virtual SendOptionsResponse(void* addInfo);

    /*!
     * @brief Sets handler action for specified url
     *
     * @param url string url address
     * @param handler pointer to handler object
     */
    void SetUrlHandler(const std::string &url,
                                        IClientConnectionHandler *handler);

private:
    const json::Value& m_config;
    int m_port;
    int m_threads;
    bool m_running;
    std::string m_path_sslcert;
    std::string m_path_sslkey;
    std::string m_sslcert;
    std::string m_sslkey;

    struct MHD_Daemon *m_daemon;

    /*! Handler object map */
    std::map<std::string, IClientConnectionHandler*> urlhandler{};

    /*! Invoked in the context of a connection to answer
     * a request from the
     * client */
    static int callback(void *cls, struct MHD_Connection *connection,
            const char *url, const char *method, const char *version,
            const char *upload_data, size_t *upload_data_size,
            void **con_cls);

    /*!
     * @brief Gets handler object for specified url
     *
     * @param url the URL requested by the client
     *
     * @return On success handler object, nullptr otherwise
     */
    IClientConnectionHandler* GetHandler(const std::string &url);

    /*!
     * @brief Handler for POST action call
     *
     * @param url the URL requested by the client
     * @param connection connection handler
     * @param client_connection client connection data
     */
    static void handle_post_action(const char* url,
                            MHD_Connection* connection,
                            struct mhd_coninfo* client_connection);

    /*!
     * @brief Check client connection is valid
     *
     * @param cls custom value selected as callback registration time
     * @param con_cls request connection data
     * @param connection connection object
     *
     * @return On success true is returned, false otherwise
     */
    static bool check_client_connection(void* cls,
                                        void** con_cls,
                                        MHD_Connection* connection);

    /*!
     * @brief Check incomming data in POST request
     *
     * @param upload_data pointer do POST data
     * @param upload_data_size data size
     * @param client_connection connection
     *
     * @return On success true is returned, false otherwise
     */
    static bool check_post_data(const char* upload_data,
                        std::size_t* upload_data_size,
                        struct mhd_coninfo* client_connection);
};
}
}
}
#endif
