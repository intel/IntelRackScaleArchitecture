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

#include "psme/rest/node/crud/managers.hpp"
#include "psme/rest/node/crud/network_service.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char Managers::TYPE[];

Managers::Managers(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Managers::TYPE)))
{ }

void Managers::set_odata_context(const json::Value& config) {

    bool is_storage_service = false;
    auto service_mode_jvalue = config["rest-server"]["storage-service-mode"];

    if (service_mode_jvalue.is_boolean()) {
        is_storage_service = config["rest-server"]["storage-service-mode"].as_bool();
    }

    if (!is_storage_service) {
        return;
    }

    json::Value json_content;

    json_content[Resource::ODATA_CONTEXT]
            = "/rest/v1/$metadata#RSAStorageManagers";
    json_content[Resource::ODATA_TYPE] = "#RSAStorageManager.1.0.0.RSAStorageManagers";
    json_content[Resource::NAME] = "Storage Managers Collection";

    get_resource().patch(json_content);
}

Managers::~Managers() { }

constexpr const char Manager::TYPE[];
constexpr const char Manager::MANAGER_FOR_DRAWERS[];
constexpr const char Manager::MANAGER_FOR_COMPUTEMODULES[];
constexpr const char Manager::MANAGER_FOR_FABRICMODULES[];
constexpr const char Manager::MANAGER_FOR_BLADES[];
constexpr const char Manager::MANAGER_FOR_SWITCHES[];
constexpr const char Manager::SIMPLE_NETWORK[];

Manager::Manager(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Manager::TYPE)))
{ }

Manager::~Manager() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<Managers> g_managers(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, "/rest/v1/$metadata#RSAManagers"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSAManager.1.0.0.RSAManagerCollection"},
        {Resource::NAME, "Manager Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                json::Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                json::Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<Manager> g_manager(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT,
                "/rest/v1/$metadata#RSAManagers/Links/Members/$entity"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSAManager.1.0.0.RSAManager"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Manager"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"ManagerType", json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {"GraphicalConsole", json::Value({Pair("Enabled", false)})},
        {"SerialConsole", json::Value({Pair("Enabled", false)})},
        {"CommandShell", json::Value({Pair("Enabled", false)})},
        {"Firmware", json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair(Manager::MANAGER_FOR_DRAWERS, json::Value::Type::ARRAY),
                Pair(Manager::MANAGER_FOR_COMPUTEMODULES, json::Value::Type::ARRAY),
                Pair(Manager::MANAGER_FOR_BLADES, json::Value::Type::ARRAY),
                Pair(Manager::MANAGER_FOR_FABRICMODULES, json::Value::Type::ARRAY),
                Pair(Manager::MANAGER_FOR_SWITCHES, json::Value::Type::ARRAY),
                Pair(NetworkService::TYPE, json::Value::Type::OBJECT),
                Pair(Manager::SIMPLE_NETWORK, json::Value::Type::OBJECT),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })},
        {Resource::ACTIONS, json::Value::Type::OBJECT}
    }});
