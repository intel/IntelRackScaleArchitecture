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

#include "psme/rest/node/crud/targets.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "psme/rest/error/server_exception.hpp"
#include "psme/rest/error/error_factory.hpp"
#include "json/value.hpp"

#include "core/service/service_factory.hpp"
#include "core/service/storage_service.hpp"

#include "core/dto/storage/add_target_dto.hpp"

#include "logger_ext.hpp"
#include "psme/rest/node/crud/logical_drives.hpp"
#include "psme/rest/node/crud/services.hpp"
#include "psme/rest/node/crud/storage_managers.hpp"

using namespace psme::rest::node;
using namespace psme::rest::resource;
using psme::rest::resource::ResourceUPtr;
using psme::rest::http::HttpHeaders;
using psme::rest::error::ServerException;
using psme::rest::error::ErrorFactory;

constexpr const char Targets::TYPE[];
constexpr const char Targets::REMOTE_TARGETS[];

Targets::Targets(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Targets::TYPE)))
{ }

Targets::~Targets() { }

std::string Targets::generate_child_id() const {
    static std::uint32_t id{0};
    return std::to_string(++id);
}

constexpr const char Target::TYPE[];

Target::Target(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Target::TYPE)))
{ }

Target::~Target() { }

namespace {
    using namespace psme::core::dto::storage;

    void process_luns(const Targets& targets, AddTargetDTO::Request& request,
                                              const json::Value& json) {
        for (const auto& lun : json) {
            try {
                const auto& node_obj =
                            targets.get_node_by_id(
                                    lun["LogicalDrive"]["@odata.id"].as_string());
                request.add_lun({node_obj.get_uuid(), lun["LUN"].as_uint()});
             } catch (const ServerException&) {
                 log_error(GET_LOGGER("rest"), "Invalid drive id: " <<
                        lun["LogicalDrive"]["@odata.id"].as_string());
                 throw ServerException(ErrorFactory::create_invalid_payload_error());
             }
        }
    }

    void
    process_addresses(const Targets& targets, AddTargetDTO::Request& request,
                                              const json::Value& json) {
        for (const auto& addr : json["Addresses"].as_array()) {
            if (addr["iSCSI"].is_object()) {
                request.set_target_iqn(addr["iSCSI"]["TargetIQN"].as_string());
                process_luns(targets, request, addr["iSCSI"]["TargetLUN"]);
            }
        }
    }

    void process_initiator(AddTargetDTO::Request& request,
                                                    const json::Value& json) {
        for (const auto& initiator : json["Initiator"].as_array()) {
            if (initiator["iSCSI"].is_object()) {
                request.set_initiator_iqn(
                        initiator["iSCSI"]["InitiatorIQN"].as_string());
            }
        }
    }

    AddTargetDTO::Request
    get_iscsi_add_request(const Targets& targets, const json::Value& json) {
        AddTargetDTO::Request request;
        try {
            process_addresses(targets, request, json);
            process_initiator(request, json);
        } catch (const json::Value::Exception&) {
            throw ServerException(ErrorFactory::create_invalid_payload_error());
        }

        return request;
    }

    void set_connection_data(json::Value& json,
                             const AddTargetDTO::Response& response) {
        for (auto& addr : json["Addresses"].as_array()) {
            if (addr["iSCSI"].is_object()) {
                addr["iSCSI"]["TargetPortalIP"] =
                                            response.get_target_address();
                addr["iSCSI"]["TargetPortalPort"] = response.get_target_port();
            }
        }
    }

    void link_logical_drives(Node& target, const json::Value& addresses) {
        for (const auto& addr : addresses) {
            if (addr["iSCSI"].is_object()) {
                for (const auto& lun : addr["iSCSI"]["TargetLUN"]) {
                    auto& logical_drive =
                        target.get_node_by_id(
                                lun["LogicalDrive"]["@odata.id"].as_string());
                    logical_drive.add_link(Targets::TYPE, target, LogicalDrives::TYPE);
                }
            }
        }
    }

    void link_storage_manager(Node& target) {
        auto* service = target.find_back_if([](const Node& parent) {
            return parent.get_type() == Service::TYPE;
        });
        if (nullptr != service) {
            Node* manager = nullptr;
            for (const auto& link : service->get_links()) {
                if (link.m_node->get_type() == StorageManager::TYPE) {
                    manager = link.m_node;
                }
            }
            if (nullptr != manager) {
                manager->add_link(Targets::REMOTE_TARGETS, target, Resource::MANAGED_BY);
            }
        }
    }
}

void Targets::post(const Request& request, Response& response) {
    json::Value json;
    json::Deserializer(request.get_body()) >> json;

    auto service = core::service::ServiceFactory::create_storage(get_gami_id());
    auto add_request = get_iscsi_add_request(*this, json);

    auto add_response = service.add_target(add_request);
    if (!add_response.is_valid()) {
        log_error(GET_LOGGER("rest"), add_response.get_error().to_string());
        throw ServerException(ErrorFactory::create_invalid_payload_error());
    }

    auto target = std::make_shared<Target>(add_response.get_target(),
                                           get_gami_id());
    set_connection_data(json, add_response);
    target->get_resource().patch(json);

    add_node(target);
    add_link(Resource::MEMBERS, *target);

    link_logical_drives(*target, json["Addresses"]);
    link_storage_manager(*target);

    target->get_resource().set_status(Status{StateType::ENABLED, HealthType::OK});
    target->get_resource().set_enumerated(EnumStatus::ENUMERATED);

    std::string location = "http://"
        + request.get_headers().get_header_values(HttpHeaders::HOST).as_string()
        + target->get_path();
    response.add_header(HttpHeaders::LOCATION, location);
    response.set_reply(http::HttpStatusCode::CREATED);
}

void Target::del(const Request&, Response& response) {
    // execute JSON-RPC command
    auto service = core::service::ServiceFactory::create_storage(get_gami_id());
    auto result = service.delete_target(get_uuid());
    if (!result.is_valid()) {
        log_error(GET_LOGGER("rest"), result.get_error().to_string());
        throw ServerException(ErrorFactory::create_invalid_payload_error());
    }
    erase(*this);
    response.set_reply(http::HttpStatusCode::NO_CONTENT);
}

void Target::patch(const Request&, Response& response) {
    auto service = core::service::ServiceFactory::create_storage(get_gami_id());
    service.set_component_attributes(get_uuid(), {});
    response.set_reply(http::HttpStatusCode::OK);
}

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<Targets> g_targets(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSARemoteTarget.1.0.0.RSARemoteTarget"},
        {Resource::NAME, "Remote Targets Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<Target> g_target(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSARemoteTarget.1.0.0.RSARemoteTarget"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Remote Target"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"Type", json::Value::Type::STRING},
        {"Addresses", json::Value::Type::ARRAY},
        {"Initiator", json::Value::Type::ARRAY},
        {Resource::ENUMERATED, "NotEnumerated"},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair{Resource::MANAGED_BY, json::Value::Type::ARRAY},
                Pair{"LogicalDrives", json::Value::Type::ARRAY},
                Pair{Resource::OEM, json::Value::Type::OBJECT}
            })}
    }});
