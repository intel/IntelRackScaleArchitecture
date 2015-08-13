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

#include "psme/rest/node/crud/processors.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char Processors::TYPE[];

Processors::Processors(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Processors::TYPE)))
{ }

Processors::~Processors() { }

constexpr const char Processor::TYPE[];

Processor::Processor(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Processor::TYPE)))
{ }

Processor::~Processor() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<Processors> g_processors(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSAProcessor.1.0.0.RSAProcessorCollection"},
        {Resource::NAME, "Processor Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
        json::Value({
            Pair(Resource::MEMBERS_ODATA_COUNT, 0),
            Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
        })}
}});

static ResourceDefinitions::Register<Processor> g_processor(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSAProcessor.1.0.0.RSAProcessor"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "CPU"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"Socket", json::Value::Type::STRING},
        {"Model", json::Value::Type::STRING},
        {"Manufacturer", json::Value::Type::STRING},
        {"ProcessorType", json::Value::Type::STRING},
        {"ProcessorArchitecture", json::Value::Type::STRING},
        {"InstructionSet", json::Value::Type::STRING},
        {"ProcessorId", json::Value::Type::OBJECT},
        {"MaxSpeedMHz", json::Value::Type::NUMBER},
        {"TotalCores", json::Value::Type::NUMBER},
        {"EnabledCores", json::Value::Type::NUMBER},
        {"TotalThreads", json::Value::Type::NUMBER},
        {"EnabledThreads", json::Value::Type::NUMBER},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
        json::Value({
            Pair(Resource::CONTAINED_BY, json::Value::Type::OBJECT),
            Pair(Resource::OEM, json::Value::Type::OBJECT)
        })}
    }});
