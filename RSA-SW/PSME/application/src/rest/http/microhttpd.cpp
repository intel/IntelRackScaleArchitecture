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

#include "psme/rest/http/microhttpd.hpp"
#include "logger/logger_factory.hpp"

#include <safe-string/safe_lib.hpp>

#include <cstring>

#ifndef MHD_HTTP_METHOD_GET
#define MHD_HTTP_METHOD_GET "GET"
#endif
#ifndef MHD_HTTP_METHOD_POST
#define MHD_HTTP_METHOD_POST "POST"
#endif
#ifndef MHD_HTTP_METHOD_PUT
#define MHD_HTTP_METHOD_PUT "PUT"
#endif
#ifndef MHD_HTTP_METHOD_PATCH
#define MHD_HTTP_METHOD_PATCH "PATCH"
#endif
#ifndef MHD_HTTP_METHOD_DELETE
#define MHD_HTTP_METHOD_DELETE "DELETE"
#endif
#ifndef MHD_HTTP_METHOD_HEAD
#define MHD_HTTP_METHOD_HEAD "HEAD"
#endif

using namespace psme::rest::http;

struct connection_info {
    connection_info() : message{} { }
    std::string message;
};

static int send_response(struct MHD_Connection* connection,
                         const Response& response) {
    struct MHD_Response* mhd_response = MHD_create_response_from_buffer(
            response.get_body().size(),
            const_cast<char*>(response.get_body().c_str()),
            MHD_RESPMEM_MUST_COPY);

    if (nullptr == mhd_response) {
        log_error(GET_LOGGER("rest"), "Cannot create response\n");
        return MHD_NO;
    }

    MHD_add_response_header(mhd_response, MHD_HTTP_HEADER_CONTENT_TYPE, response.get_mime());

    for (const auto& header: response.get_headers()) {
        const auto& header_name = header.first;
        const auto& header_values = header.second;
        MHD_add_response_header(mhd_response, header_name.c_str(),
                const_cast<char*>(header_values.as_string().c_str()));
    }

    /* Response to client */
    int ret = MHD_queue_response(connection, response.get_status(), mhd_response);
    MHD_destroy_response(mhd_response);

    return ret;
}

static int add_request_headers(void* cls, enum MHD_ValueKind kind,
        const char* key, const char* value) {
    (void)kind;

    Request* request = static_cast<Request*>(cls);
    request->add_header(key, value);

    return MHD_YES;
}

static int request_with_data(Server::Method http_method,
        void* cls, struct MHD_Connection* connection,
        const char* url, const char* method, const char* version,
        const char* upload_data, size_t* upload_data_size, void** con_cls) {
    (void)cls;
    (void)url;
    (void)method;
    (void)version;

    struct connection_info* con_info =
        static_cast<struct connection_info*>(*con_cls);

    if (nullptr == con_info) {
        con_info = new struct connection_info;
        if (nullptr == con_info) {
            return MHD_NO;
        }
        *con_cls = con_info;
        return MHD_YES;
    }

    if (0 != *upload_data_size) {
        con_info->message.append(upload_data, *upload_data_size);
        *upload_data_size = 0;
        return MHD_YES;
    }

    Request request(url, con_info->message);
    MHD_get_connection_values(connection, MHD_HEADER_KIND,
                              &add_request_headers, &request);
    Response response;
    static_cast<MicroHttpd*>(cls)->call(http_method, request, response);

    delete con_info;
    *con_cls = nullptr;

    return send_response(connection, response);
}

static int request_no_data(Server::Method http_method,
        void* cls, struct MHD_Connection* connection,
        const char* url, const char* method, const char* version,
        const char* upload_data, size_t* upload_data_size, void** con_cls) {

    (void)cls;
    (void)url;
    (void)method;
    (void)version;
    (void)upload_data;

    if (0 != *upload_data_size) {
        /* Upload data !? */
        return MHD_NO;
    }
    *con_cls = nullptr;

    Request request(url, "");
    MHD_get_connection_values(connection, MHD_HEADER_KIND,
                              &add_request_headers, &request);
    Response response;
    static_cast<MicroHttpd*>(cls)->call(http_method, request, response);

    return send_response(connection, response);
}

static int access_handler_callback(void* cls, struct MHD_Connection *connection,
    const char* url, const char* method, const char* version,
    const char* upload_data, size_t* upload_data_size, void** con_cls) {

    log_debug(GET_LOGGER("rest"), "HTTP Method " << method);

    if (0 == strcmp(method, MHD_HTTP_METHOD_GET)) {
        return request_no_data(Server::Method::GET,
                cls, connection, url, method, version,
                upload_data, upload_data_size, con_cls);
    } else if (0 == strcmp(method, MHD_HTTP_METHOD_POST)) {
        return request_with_data(Server::Method::POST,
                cls, connection, url, method, version,
                upload_data, upload_data_size, con_cls);
    } else if (0 == strcmp(method, MHD_HTTP_METHOD_PUT)) {
        return request_with_data(Server::Method::PUT,
                cls, connection, url, method, version,
                upload_data, upload_data_size, con_cls);
    } else if (0 == strcmp(method, MHD_HTTP_METHOD_PATCH)) {
        return request_with_data(Server::Method::PATCH,
                cls, connection, url, method, version,
                upload_data, upload_data_size, con_cls);
    } else if (0 == strcmp(method, MHD_HTTP_METHOD_DELETE)) {
        return request_no_data(Server::Method::DELETE,
                cls, connection, url, method, version,
                upload_data, upload_data_size, con_cls);
    } else if (0 == strcmp(method, MHD_HTTP_METHOD_HEAD)) {
        return request_no_data(Server::Method::HEAD,
                cls, connection, url, method, version,
                upload_data, upload_data_size, con_cls);
    }

    Request request(url, "");
    MHD_get_connection_values(connection, MHD_HEADER_KIND,
                              &add_request_headers, &request);
    Response response;
    static_cast<MicroHttpd*>(cls)->call(
            Server::Method::NOT_ALLOWED, request, response);
    return send_response(connection, response);
}

MicroHttpd::MicroHttpd(const string& url) :
    Server(url),
    m_daemon(nullptr) { }

MicroHttpd::~MicroHttpd() {
    if (nullptr != m_daemon) {
        MHD_stop_daemon(m_daemon);
        m_daemon = nullptr;
    }
}

static std::string parse_url(string url, uint16_t& port, bool& use_ssl);

void MicroHttpd::open() {
    uint16_t port = 80;
    bool use_ssl = false;

    std::string url = parse_url(get_url(), port, use_ssl);
    start_daemon(port, use_ssl);

    log_debug(GET_LOGGER("rest"),
        "HTTP Server daemon started at " << url << ":" << port);
}

void MicroHttpd::close() {
    if (nullptr != m_daemon) {
        MHD_stop_daemon(m_daemon);
        m_daemon = nullptr;

        log_debug(GET_LOGGER("rest"), "HTTP Server daemon stopped\n");
    }
}

static constexpr char KEY_PEM[] = R"(-----BEGIN RSA PRIVATE KEY-----
MIICXQIBAAKBgQDN/AB8IVvPkxZWofHTwogqwGY2x6eo31Q9htnWChUTm+iei8U/
uUpHNOzpXHpVUa5j2TrCeoVyogcefBSlSlMh78jrL30KvpU2wpJWkHa5b3l2IMRn
+sOsFY6JDCZa8v7ASOZfdA/FftlfUmm9OaBMo8oYn9fSbtqc2wSdGlT0PQIDAQAB
AoGAVgV8ilpABkLN3GYLIL0nsnrg9NWSTfLeMkejp0BUpvjAP53tRPxzaaNPoPEO
4ZyX8Hjikh8qpcHGqVN2J8UPS+e+rVCmm0UBu02C0nyZ08pWdLr7yklX70XpwDsN
1+/Ap0Rc2cw5DntJ05CnKZ1yrnOYox73RchJAyVkebsbdaECQQDzk+zobvG/Z07u
8wI1mZSRtw4N+ylfekh+06fyqiCxEL0PUWpqGNDV0AhZGB7O5O26U7ct586cKAwb
2Q/RjnmVAkEA2H1EsrdfEY8pcxdwkijgeIgrirRRYzDW2u/Dc0eTcsdAOrGf5r19
We4HSSLUUPgdQZBIljd4RU9IIW8JjSh2CQJBAMg3Oa89Mr2wP3OukIxad4LnQ0g+
wZ1XuKt3cz/GlCS3q5AhziTzzQVTgOkrKHfMOZTFJ3c2aeB2Eq35WEVapZkCQFn3
POxUHfLrHpuQmxxyVk5gsE+U/dmNu0wCpaOrlA/3065QkJXQG16a93YiPj9jzKyp
bDDY3mdXOi+CuGJr9RkCQQDjKVDJXdjs0SkIjnhny9ZnW/pba2EvcKFcahSv5gwH
LFk5HE3YcTRDdi0ClAWzw6DtIF5NVkSBp0rEnT5jyPlr
-----END RSA PRIVATE KEY-----)";

static constexpr char CERT_PEM[] = R"(-----BEGIN CERTIFICATE-----
MIICUjCCAbugAwIBAgIJAMUSRk2rhG2jMA0GCSqGSIb3DQEBCwUAMEIxCzAJBgNV
BAYTAlBMMRIwEAYDVQQIDAlQb21vcnNraWUxDzANBgNVBAcMBkdkYW5zazEOMAwG
A1UECgwFSW50ZWwwHhcNMTUwNTEzMTExMzMyWhcNMTYwNTEyMTExMzMyWjBCMQsw
CQYDVQQGEwJQTDESMBAGA1UECAwJUG9tb3Jza2llMQ8wDQYDVQQHDAZHZGFuc2sx
DjAMBgNVBAoMBUludGVsMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDN/AB8
IVvPkxZWofHTwogqwGY2x6eo31Q9htnWChUTm+iei8U/uUpHNOzpXHpVUa5j2TrC
eoVyogcefBSlSlMh78jrL30KvpU2wpJWkHa5b3l2IMRn+sOsFY6JDCZa8v7ASOZf
dA/FftlfUmm9OaBMo8oYn9fSbtqc2wSdGlT0PQIDAQABo1AwTjAdBgNVHQ4EFgQU
ANCuFq4O/Q8FJTJS7gxNF1qJ5GQwHwYDVR0jBBgwFoAUANCuFq4O/Q8FJTJS7gxN
F1qJ5GQwDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQsFAAOBgQCvggKBn2pbgOgH
lP9HVvLar1pMwkLrXM4IWc8pK6JopRxz96KAQUgMp20xKWEA99LWnHkTEMCgpUcS
vPZRmoXq/Lt31gYelX5kDviuwhuOhi8ptO8t5lWHmZGddvigYMeDFIf0m4Q6aRse
lvPtKDza9KfXAMtSv7797/nvxsxhxg==
-----END CERTIFICATE-----)";

void
MicroHttpd::start_daemon(uint16_t port, bool use_ssl) {
    if (use_ssl) {
        m_daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY | MHD_USE_SSL, port,
            nullptr, nullptr,
            access_handler_callback, this,
            MHD_OPTION_HTTPS_MEM_KEY, KEY_PEM,
            MHD_OPTION_HTTPS_MEM_CERT, CERT_PEM,
            MHD_OPTION_END);
    } else {
        m_daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port,
            nullptr, nullptr,
            access_handler_callback, this,
            MHD_OPTION_END);
    }

    if (nullptr == m_daemon) {
        log_error(GET_LOGGER("rest"), " Cannot start REST HTTP Server daemon\n");
        throw std::runtime_error("Cannot start REST HTTP Server daemon");
    }
}


static std::string
parse_url(string url, uint16_t& port, bool& use_ssl) {
    size_t pos = 0;

    if (0 == url.find("http://")) {
        pos = strnlen_s("http://", RSIZE_MAX_STR);
    } else if (0 == url.find("https://")) {
        pos = strnlen_s("https://", RSIZE_MAX_STR);
        use_ssl = true;
    }

    pos = url.find_first_of(':', pos);
    if (std::string::npos != pos) {
        size_t count;
        port = static_cast<uint16_t>(std::stoi(url.substr(pos + 1), &count));
        url.erase(pos, count + 1);
    }

    return url;
}
