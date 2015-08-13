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

#include "psme/rest/node/crud/storage_controllers.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char StorageControllers::TYPE[];

StorageControllers::StorageControllers(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(StorageControllers::TYPE)))
{ }

StorageControllers::~StorageControllers() { }

constexpr const char StorageController::TYPE[];

StorageController::StorageController(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(StorageController::TYPE)))
{ }

StorageController::~StorageController() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<StorageControllers> g_controllers(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE,
            "#RSAStorageController.1.0.0.RSAStorageControllersCollection"},
        {Resource::NAME, "StorageControllers Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
        json::Value({
            Pair(Resource::MEMBERS_ODATA_COUNT, 0),
            Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
        })}
    }});

static ResourceDefinitions::Register<StorageController> g_controller(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE,
            "#RSAStorageController.1.0.0.RSAStorageController"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "StorageController"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"Interface", json::Value::Type::STRING},
        {"DriveCount", json::Value::Type::NUMBER},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::CONTAINED_BY, json::Value::Type::OBJECT),
                Pair("Drives", json::Value::Type::OBJECT),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })}
    }});
