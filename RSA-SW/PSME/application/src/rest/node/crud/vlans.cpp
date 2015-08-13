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

#include "psme/rest/node/crud/vlans.hpp"
#include "psme/rest/node/crud/utils.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "core/service/service_factory.hpp"
#include "core/service/network_service.hpp"
#include "psme/rest/error/error_factory.hpp"
#include "psme/rest/error/server_exception.hpp"
#include "json/value.hpp"
#include "psme/rest/node/crud/switches.hpp"
#include "psme/rest/node/crud/switch_ports.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;
using psme::rest::http::HttpHeaders;
using psme::rest::error::ServerException;
using psme::rest::error::ErrorFactory;

constexpr const char Vlans::TYPE[];

Vlans::Vlans(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Vlans::TYPE)))
{ }

Vlans::~Vlans() { }

using json::Pair;

namespace {
    constexpr const char VlanId[] = "VLANId";
    constexpr const char Tagged[] = "Tagged";

json::Value validate_request(const Request& request) {
    using namespace psme::rest::error;

    json::Value schema({
        Pair(VlanId, json::Value({
            Pair("validator", true),
            Pair("type", "uint")})
        ),
        Pair(Tagged, json::Value({
            Pair("validator", true),
            Pair("type", "bool")})
        )
    });
    auto json = validate_request_body(request, schema);
    if (!json.is_member(VlanId)) {
        log_error(GET_LOGGER("rest"), " Bad request, missing property :" << VlanId);
        ServerError error = ErrorFactory::create_invalid_payload_error();
        throw ServerException(error);
    }
    return json;
}
}

using namespace psme::rest::resource;

void Vlans::post(const Request& request, Response& response) {
    auto json = validate_request(request);

    // execute JSON-RPC command
    auto service = psme::core::service::ServiceFactory::create_network(get_gami_id());

    auto* switch_node = find_back_if([](const Node& parent) {
        return parent.get_type() == Switch::TYPE;});
    auto* switch_port = find_back_if([](const Node& parent) {
        return parent.get_type() == SwitchPort::TYPE;});

    if (nullptr == switch_node || nullptr == switch_port) {
        throw std::runtime_error("Tree is not properly initialized.");
    }

    auto result = service.add_port_vlan(switch_node->get_uuid(), // switch uuid
                  switch_port->get_uuid(), // switch port_identifier,
                  json[VlanId].as_uint(),
                  json.is_member(Tagged) ? json[Tagged].as_bool() : false,
                  psme::core::dto::OEMDataDTO::Request());

    if (result.is_valid()) {
        auto vlan = std::make_shared<Vlan>(result.get_vlan_identifier(), get_gami_id());
        add_node(vlan);
        add_link(Resource::MEMBERS, *vlan);
        vlan->get_resource().set_status(Status{StateType::ENABLED, HealthType::OK});
        vlan->get_resource().set_property(VlanId, json[VlanId].as_uint());

        std::string location = "http://"
                + request.get_headers().get_header_values(HttpHeaders::HOST).as_string()
                + vlan->get_path();
        response.add_header(HttpHeaders::LOCATION, location);
        response.set_reply(http::HttpStatusCode::CREATED);
    } else {
        const auto& error = result.get_error();
        log_error(GET_LOGGER("rest"),  error.to_string());
        throw ServerException(ErrorFactory::create_invalid_payload_error());
    }
}

constexpr const char Vlan::TYPE[];

Vlan::Vlan(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Vlan::TYPE)))
{ }

Vlan::~Vlan() { }

void Vlan::del(const Request& request, Response& response) {
    (void)request;

    // execute JSON-RPC command
    auto service = psme::core::service::ServiceFactory::create_network(get_gami_id());

    auto* switch_node = find_back_if([](const Node& parent) {
        return parent.get_type() == Switch::TYPE;});
    auto* switch_port = find_back_if([](const Node& parent) {
        return parent.get_type() == SwitchPort::TYPE;});

    if (nullptr == switch_node || nullptr == switch_port) {
        throw std::runtime_error("Tree is not properly initialized.");
    }

    auto result = service.delete_port_vlan(switch_node->get_uuid(), // switch uuid
                  switch_port->get_uuid(), // switch port_identifier,
                  get_uuid(),
                  psme::core::dto::OEMDataDTO::Request());

    if (result.is_valid()) {
        erase(*this);
        response.set_reply(http::HttpStatusCode::NO_CONTENT);
    } else {
        const auto& error = result.get_error();
        log_error(GET_LOGGER("rest"),  error.to_string());
        throw ServerException(ErrorFactory::create_invalid_payload_error());
    }
}


static ResourceDefinitions::Register<Vlans> g_switch_ports(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE,
            "#VLanNetworkInterface.0.94.0.VLANNetworkInterfaces"},
        {Resource::NAME, "VLAN Network Interface Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
          {Resource::LINKS,
        json::Value({
            Pair(Resource::MEMBERS_ODATA_COUNT, 0),
            Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
        })}
    }});

static ResourceDefinitions::Register<Vlan> g_switch_port(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE,
                "#VLanNetworkInterface.1.0.0.VLANNetworkInterface"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "VLAN"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::DESCRIPTION, json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {"VLANEnable", true},
        {VlanId, json::Value::Type::NUMBER},
        {"IPv4Address", json::Value::Type::ARRAY},
        {"IPv6Address", json::Value::Type::ARRAY},
        {Resource::LINKS, json::Value({
        Pair{"NetworkInterface", json::Value::Type::ARRAY }})}}});
