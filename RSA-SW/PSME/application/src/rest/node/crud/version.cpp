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

#include "psme/rest/node/crud/version.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/resource/resource_utils.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"

#include "json/value.hpp"
#include "configuration/utils.hpp"
#include "uuid++.hh"
#include <sys/stat.h>

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char Version::TYPE[];

namespace {
    constexpr const char UUID_FILE_PROP[] = "service-uuid-file";
    constexpr const char UUID_PROP[] = "UUID";

    bool file_exists(const string& file_name) {
        struct stat buf;
        return (0 == stat(file_name.c_str(), &buf));
    }

    void generate_uuid_file(const string& file_name, json::Value& uuid_json) {
        uuid u;
        u.make(UUID_MAKE_V1);
        uuid_json[UUID_PROP] = std::string(u.string());
        try {
            configuration::json_to_file(file_name, uuid_json);
        } catch (const std::ios_base::failure& e) {
            log_error(GET_LOGGER("rest"),
                " Error writing Service UUID file: " << file_name);
            throw e;
        }
    }

    bool is_valid_uuid(const json::Value& uuid_json) {
        return uuid_json.is_member(UUID_PROP)
                && uuid_json[UUID_PROP].is_string();
    }
}

Version::Version(const string& uuid, const string& gami_id, const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(TYPE)))
{ }

Version::~Version() {
}

void Version::get(const Request&, Response& response) {
    // copy resource for safe time modification
    auto r = get_resource();
    r.set_property(Resource::ID, "RootService");
    r.set_property("Time", resource::ResourceUtils::get_time_with_zone());
    r.update_links(*this);
    response.set_reply(psme::rest::http::HttpStatusCode::OK, r.as_string());
}

void Version::handle_service_uuid(const json::Value& config) {
    const string uuid_file = config[UUID_FILE_PROP].as_string();
    try {
        json::Value uuid_json;
        if (!file_exists(uuid_file)) {
            log_info(GET_LOGGER("rest"),
                " Generating Service UUID file: " << uuid_file);
            generate_uuid_file(uuid_file, uuid_json);
        } else {
            log_info(GET_LOGGER("rest"),
                " Reading Service UUID file: " << uuid_file);
            configuration::file_to_json(uuid_file, uuid_json);
        }
        if (is_valid_uuid(uuid_json)) {
            get_resource().set_property(UUID_PROP,
                    uuid_json[UUID_PROP].as_string());
            log_info(GET_LOGGER("rest"),
                " Service UUID: " << uuid_json[UUID_PROP].as_string());
        } else {
            log_error(GET_LOGGER("rest"), " No valid Service UUID found.");
        }
    } catch (const std::ios_base::failure& e) {
        log_error(GET_LOGGER("rest"),
                " Error processing Service UUID file: " << uuid_file
                << " error: " << e.what());
    } catch (...) {
        log_error(GET_LOGGER("rest"), " Cannot parse Service UUID.");
    }
}

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<Version> g(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, "/rest/v1/$metadata#ServiceRoot"},
        {Resource::ODATA_ID, "/rest/v1"},
        {Resource::ODATA_TYPE, "#RSAServiceRoot.1.0.0.RSAServiceRoot"},
        {Resource::ID, "RootService"},
        {Resource::NAME, "RSA Service Root"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"Time", json::Value::Type::STRING},
        {"ServiceVersion", "1.0.0"},
        {"UUID", "00000000-0000-0000-0000-000000000000"},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::CHASSIS, json::Value::Type::OBJECT),
                Pair("Managers", json::Value::Type::OBJECT),
                Pair("Services", json::Value::Type::OBJECT)
            })}
    }});
