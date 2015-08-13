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

#include "psme/rest/node/crud/blades.hpp"
#include "psme/rest/node/crud/utils.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/error/error_factory.hpp"
#include "psme/rest/error/server_exception.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "core/service/service_factory.hpp"
#include "core/service/compute_service.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char Blades::TYPE[];

Blades::Blades(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Blades::TYPE)))
{ }

Blades::~Blades() { }

constexpr const char Blade::TYPE[];
constexpr const char Blade::PROP_NAME_BOOT[];
constexpr const char Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_ENABLED[];
constexpr const char Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_TARGET[];
constexpr const char Blade::Actions::Reset::PROP_NAME_RESET_TYPE[];
constexpr const char Blade::Actions::Reset::PROP_NAME_POWER[];
constexpr const char Blade::Actions::Reset::RESET_TYPE_CONTINUOUS[];

namespace {
    using psme::core::dto::compute::BladeAttributesDTO;
    void set_boot_parameters(BladeAttributesDTO::Request& req, Resource& res) {
        const auto boot_json = res.as_json()[Blade::PROP_NAME_BOOT];
        const auto boot_override_json =
            boot_json[Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_ENABLED];
        const auto boot_source_target_json =
            boot_json[Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_TARGET];

        if (Blade::Actions::Reset::RESET_TYPE_CONTINUOUS == boot_override_json) {
            req.set_boot_override(boot_override_json.as_string());
            req.set_boot_override_target(boot_source_target_json.as_string());
        } else {
            req.set_boot_override("Disabled");
            req.set_boot_override_target("None");
        }
    }

    json::Value validate_blade_patch_request(const Request& request) {
        using json::Pair;
        using namespace psme::rest::error;
        json::Value schema({
            Pair(Blade::PROP_NAME_BOOT, json::Value({
                Pair(Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_ENABLED, json::Value(
                {
                    Pair("validator", true),
                    Pair("type", "string"),
                    Pair("anyof", json::Value{"Disabled",
                                              "Once",
                                              "Continuous"})
                })),
                Pair(Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_TARGET, json::Value(
                {
                    Pair("validator", true),
                    Pair("type", "string"),
                    Pair("anyof", json::Value{"None", "Pxe", "Hdd"})
                }))
            }))
        });
        auto json = validate_request_body(request, schema);
        std::ostringstream err;
        if (!json.is_member(Blade::PROP_NAME_BOOT)) {
            err << Blade::PROP_NAME_BOOT;
        } else {
            if (!json[Blade::PROP_NAME_BOOT].is_member(
                        Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_ENABLED)) {
                err << " " << Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_ENABLED;
            }
            if (!json[Blade::PROP_NAME_BOOT].is_member(
                        Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_TARGET)) {
                err << " " << Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_TARGET;
            }
        }
        if (!err.str().empty()) {
            log_error(GET_LOGGER("rest"), " Bad request, missing properties :"
                   << err.str());
            ServerError error = ErrorFactory::create_invalid_payload_error();
            throw ServerException(error);
        }
        return json;
    }

    json::Value validate_blade_reset_post_request(const Request& request) {
        using json::Pair;
        using namespace psme::rest::error;

        json::Value schema({
            Pair(Blade::Actions::Reset::PROP_NAME_RESET_TYPE, json::Value({
                Pair("validator", true),
                Pair("type", "string"),
                Pair("anyof", json::Value{"On", "ForceOff", "SoftShutdown"})
            }))
        });

        auto json = validate_request_body(request, schema);

        std::ostringstream err;
        if (!json.is_member(Blade::Actions::Reset::PROP_NAME_RESET_TYPE)) {
            err << Blade::Actions::Reset::PROP_NAME_RESET_TYPE;
        }

        if (!err.str().empty()) {
            log_error(GET_LOGGER("rest"), " Bad request, missing properties :"
                   << err.str());
            ServerError error = ErrorFactory::create_invalid_payload_error();
            throw ServerException(error);
        }
        return json;
    }

    Node* get_blade_node(const Node* node) {
        const auto* actions = node->get_back();
        if (!actions) {
            throw std::runtime_error("Cannot get Actions node"
                                     "from Reset node.") ;
        }

        auto blade = actions->get_back();
        if (!blade) {
            throw std::runtime_error("Cannot get Blade node "
                                     "from Actions node.") ;
        }
        return blade;
    }
}

Blade::Blade(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Blade::TYPE)))
{
    auto actions = std::make_shared<Actions>();
    add_node(actions);
}

Blade::~Blade() { }

void Blade::patch(const Request& request, Response& response) {
    using core::service::ServiceFactory;
    using psme::core::dto::compute::BladeAttributesDTO;

    auto json = validate_blade_patch_request(request);

    const auto& boot_override =
        json[PROP_NAME_BOOT][PROP_NAME_BOOT_SOURCE_OVERRIDE_ENABLED].as_string();

    const auto& boot_override_target =
        json[PROP_NAME_BOOT][PROP_NAME_BOOT_SOURCE_OVERRIDE_TARGET].as_string();

    BladeAttributesDTO::Request request_dto;
    request_dto.set_component(get_uuid());
    request_dto.set_boot_override(boot_override);
    request_dto.set_boot_override_target(boot_override_target);

    auto compute_service = ServiceFactory::create_compute(get_gami_id());
    compute_service.set_blade_attributes(request_dto);

    // Updates Resource on the REST.
    json::Value content;
    content[PROP_NAME_BOOT][PROP_NAME_BOOT_SOURCE_OVERRIDE_ENABLED]
            = boot_override;
    content[PROP_NAME_BOOT][PROP_NAME_BOOT_SOURCE_OVERRIDE_TARGET]
            = boot_override_target;

    get_resource().patch(content);

    response.set_reply(http::HttpStatusCode::OK);
}

constexpr const char Blade::Actions::TYPE[];

Blade::Actions::Actions(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Blade::Actions::TYPE)))
{
    auto reset = std::make_shared<Reset>();
    add_node(reset);
}

Blade::Actions::~Actions() {}

constexpr const char Blade::Actions::Reset::TYPE[];
constexpr const char Blade::Actions::Reset::ALLOWABLE_VALUES[];

Blade::Actions::Reset::Reset(const string& uuid,
                             const string& gami_id,
                             const string& id)
    : Node(uuid, gami_id, id,
           ResourceUPtr(new Resource(Blade::Actions::Reset::TYPE)))
{ }

Blade::Actions::Reset::~Reset() { }

void Blade::Actions::Reset::post(const Request& request, Response& response) {
    using core::service::ServiceFactory;
    using psme::core::dto::compute::BladeAttributesDTO;

    auto json = validate_blade_reset_post_request(request);
    auto blade = get_blade_node(this);

    const auto& power_state =
        json[Blade::Actions::Reset::PROP_NAME_RESET_TYPE].as_string();

    BladeAttributesDTO::Request request_dto;
    request_dto.set_component(blade->get_uuid());
    request_dto.set_power_state(power_state);

    set_boot_parameters(request_dto, blade->get_resource());

    auto compute_service = ServiceFactory::create_compute(blade->get_gami_id());
    compute_service.set_blade_attributes(request_dto);

    response.set_reply(http::HttpStatusCode::OK);
}

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<Blades> g_blades(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, "/rest/v1/$metadata#RSABlades"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSABlade.1.0.0.RSABladeCollection"},
        {Resource::NAME, "Blade Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<Blade> g_blade(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT,
                "/rest/v1/$metadata#RSABlades/Links/Members/$entity"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSABlade.1.0.0.RSABlade"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Blade"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"SystemType", json::Value::Type::STRING},
        {"Manufacturer", json::Value::Type::STRING},
        {"Model", json::Value::Type::STRING},
        {"SerialNumber", json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {"Bios", json::Value::Type::OBJECT},
        {"Processors", json::Value::Type::OBJECT},
        {"Memory", json::Value::Type::OBJECT},
        {Resource::ENUMERATED, json::Value::Type::STRING},
        {"StorageCapable", json::Value::Type::BOOLEAN},
        {"StorageControllersCount", json::Value::Type::NUMBER},
        {Blade::PROP_NAME_BOOT,
            json::Value({
                Pair(Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_ENABLED, "Disabled"),
                Pair(Blade::PROP_NAME_BOOT_SOURCE_OVERRIDE_TARGET, "None"),
                Pair("BootSourceOverrideSupported", json::Value{"None","Pxe",
                        "Floppy","Cd","Usb","Hdd","BiosSetup","Utilities",
                        "Diags","UefiTarget"})
            })},
        {Resource::LINKS,
            json::Value({
                Pair("Processors", json::Value::Type::OBJECT),
                Pair("Memory", json::Value::Type::OBJECT),
                Pair(Resource::CHASSIS, json::Value::Type::ARRAY),
                Pair("StorageControllers", json::Value::Type::OBJECT),
                Pair("EthernetInterfaces", json::Value::Type::OBJECT),
                Pair(Resource::MANAGED_BY, json::Value::Type::ARRAY),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })},
        {Resource::ACTIONS, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT}
    }});

static ResourceDefinitions::Register<Blade::Actions::Reset> g_reset(ResourceDef{
    PropertyVec{
        {Resource::ACTION_TARGET, json::Value::Type::STRING},
        {Blade::Actions::Reset::ALLOWABLE_VALUES,
                        json::Value{"On", "ForceOff", "SoftShutdown"}}
    }});
