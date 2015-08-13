/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, NetworkService 2.0 (the "License");
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

#include "psme/rest/node/crud/network_service.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/resource/resource_utils.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char NetworkService::TYPE[];

NetworkService::NetworkService(const string& uuid, const string& gami_id, const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(TYPE)))
{ }

NetworkService::~NetworkService() {
}

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<NetworkService> g(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE,
                "#ManagerNetworkService.1.0.0.ManagerNetworkService"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Manager Network Service"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::DESCRIPTION, "Manager Network Service Status"},
        {Status::STATUS, json::Value::Type::OBJECT},
        {"HostName", json::Value::Type::STRING},
        {"FQDN", json::Value::Type::STRING},
        {"HTTP", json::Value({Pair("Enabled", false)})},
        {"HTTPS",json::Value({Pair("Enabled", false)})},
        {"IPMI", json::Value({Pair("Enabled", false)})},
        {"SSH", json::Value({Pair("Enabled", false)})},
        {"SNMP", json::Value({Pair("Enabled", false)})},
        {"VirtualMedia", json::Value({Pair("Enabled", false)})},
        {"SSDP", json::Value({Pair("Enabled", false)})},
        {"Telnet", json::Value({Pair("Enabled", false)})},
        {"KVMIP", json::Value({Pair("Enabled", false)})}}});
