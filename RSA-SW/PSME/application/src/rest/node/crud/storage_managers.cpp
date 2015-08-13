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
#include "psme/rest/node/crud/services.hpp"
#include "psme/rest/node/crud/storage_managers.hpp"
#include "psme/rest/node/crud/logical_drives.hpp"
#include "psme/rest/node/crud/network_service.hpp"
#include "psme/rest/node/crud/targets.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char StorageManager::TYPE[];
constexpr const char StorageManager::SIMPLE_NETWORK[];

StorageManager::StorageManager(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(StorageManager::TYPE)))
{ }

StorageManager::~StorageManager() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<StorageManager> g_manager(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT,
                "/rest/v1/$metadata#RSAStorageManagers/Links/members/$entity"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSAStorageManager.1.0.0.RSAStorageManager"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "RSA Storage Manager"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"ManagerType", json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {"Firmware", json::Value::Type::OBJECT},
        {"GraphicalConsole", json::Value::Type::OBJECT},
        {"SerialConsole", json::Value::Type::OBJECT},
        {"CommandShell", json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair(Services::TYPE, json::Value::Type::ARRAY),
                Pair(Targets::REMOTE_TARGETS, json::Value::Type::ARRAY),
                Pair(LogicalDrives::TYPE, json::Value::Type::ARRAY),
                Pair(NetworkService::TYPE, json::Value::Type::OBJECT),
                Pair(StorageManager::SIMPLE_NETWORK, json::Value::Type::OBJECT),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })},
        {Resource::ACTIONS, json::Value::Type::OBJECT}
    }});

