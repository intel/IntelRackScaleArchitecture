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

#include "psme/rest/node/crud/logical_drives.hpp"
#include "psme/rest/node/crud/targets.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "psme/rest/error/server_exception.hpp"
#include "psme/rest/error/error_factory.hpp"
#include "json/value.hpp"
#include "psme/rest/node/crud/utils.hpp"
#include "core/dto/storage/add_logical_drive_dto.hpp"
#include "core/service/service_factory.hpp"
#include "core/service/storage_service.hpp"
#include "psme/rest/node/crud/services.hpp"
#include "psme/rest/node/crud/storage_managers.hpp"


using namespace psme::rest::node;
using namespace psme::rest::resource;
using psme::rest::resource::ResourceUPtr;
using psme::rest::http::HttpHeaders;
using psme::rest::error::ServerException;
using psme::rest::error::ErrorFactory;

constexpr const char LogicalDrives::TYPE[];

LogicalDrives::LogicalDrives(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(LogicalDrives::TYPE)))
{ }

LogicalDrives::~LogicalDrives() { }

std::string LogicalDrives::generate_child_id() const {
    static std::uint32_t id{0};
    return std::to_string(++id);
}

constexpr const char LogicalDrive::TYPE[];

LogicalDrive::LogicalDrive(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(LogicalDrive::TYPE)))
{ }

LogicalDrive::~LogicalDrive() { }



using namespace psme::core::dto::storage;

constexpr const char TYPE_FIELD[] = "Type";
constexpr const char TYPE_DEFAULT_VALUE[] = "LVM";
constexpr const char CAPACITY_GB[] = "CapacityGB";
constexpr const char MODE[] = "Mode";
constexpr const char MASTER_DRIVE[] = "MasterDrive";
constexpr const char SNAPSHOT[] = "Snapshot";
constexpr const char PROTECTED[] = "Protected";
constexpr const char LINKS[] = "Links";
constexpr const char ODATA[] = "@odata.id";
constexpr const char ASSET_TAG[] = "AssetTag";
constexpr const char IMAGE[] = "Image";
constexpr const char BOOTABLE[] = "Bootable";
constexpr const char MODE_LV[] = "LV";
constexpr const char MANAGERS_PATH[] = "/rest/v1/Managers";
constexpr const char USED_BY[] = "UsedBy";
constexpr const char PHYSICAL_DRIVES[] = "PhysicalDrives";


using json::Pair;
using namespace psme::rest::error;

namespace {

    void validate_master_drive(const LogicalDrives& logical_drives,
                                                const json::Value& json) {
        const auto& master_drive = json[LINKS][MASTER_DRIVE];
        try {
            if (!master_drive.is_member(ODATA)) {
                log_error(GET_LOGGER("rest"), "Missing property :" << ODATA);
                throw ServerException(ErrorFactory::create_invalid_payload_error());
            }
            if (master_drive[ODATA].as_string().empty()) {
                log_error(GET_LOGGER("rest"), "Empty Master drive path");
                throw ServerException(ErrorFactory::create_invalid_payload_error());
            }
            logical_drives.get_node_by_id(master_drive[ODATA].as_string());
        } catch (const std::exception&) {
            log_error(GET_LOGGER("rest"), "No Master drive");
            throw ServerException(ErrorFactory::create_invalid_payload_error());
        }
    }

    void validate_logical_drives(const LogicalDrives& logical_drives,
                                                const json::Value& json) {
        bool logical_drive_found{false};
        const auto& logical_drives_json = json[LINKS][LogicalDrives::TYPE];
        try {
            for(const auto& logical_drive : logical_drives_json.as_array()){
                if (logical_drive.is_member(ODATA) &&
                            nullptr != logical_drive[ODATA] &&
                            !logical_drive[ODATA].as_string().empty()) {
                    logical_drive_found = true;
                    logical_drives.get_node_by_id(logical_drive[ODATA].as_string());
                    break;
                }
            }
        } catch (const std::exception& ex) {
            log_error(GET_LOGGER("rest"), "No logical drive found, "
                    << ex.what());
            throw ServerException(ErrorFactory::create_invalid_payload_error());
        }
        if(!logical_drive_found) {
            log_error(GET_LOGGER("rest"), "No logical drive found");
            throw ServerException(ErrorFactory::create_invalid_payload_error());
        }
    }

    const json::Value validate_request(const LogicalDrives& logical_drives,
                                        const Request& request) {
        std::map<std::string, std::string> fields = {
            {MODE, "string"},
            {CAPACITY_GB, "uint"},
            {PROTECTED, "bool"}
        };
        json::Value schema;
        for (const auto& filed : fields) {
            schema.push_back(Pair(filed.first, json::Value({
                    Pair("validator", true),
                    Pair("type", filed.second)})
            ));
        }
        schema.push_back(Pair(LogicalDrives::TYPE, json::Value({
            Pair("validator", true),
            Pair("type", "array")
        })));
        schema.push_back(Pair(MASTER_DRIVE, json::Value({
            Pair("validator", true),
            Pair("type", "object")
        })));
        auto json = validate_request_body(request, schema);
        for (const auto& field : fields) {
            if (!json.is_member(field.first)) {
                log_error(GET_LOGGER("rest"), "Missing property : " << field.first);
                throw ServerException(ErrorFactory::create_invalid_payload_error());
            }
        }
        if (MODE_LV != json[MODE]) {
            log_error(GET_LOGGER("rest"), "Unsupported mode : " << json[MODE]);
            throw ServerException(ErrorFactory::create_invalid_payload_error());
        }

        validate_master_drive(logical_drives, json);
        validate_logical_drives(logical_drives, json);

        return json;
    }

    AddLogicalDriveDTO::Request create_request(const LogicalDrives& logical_drives,
                                                const json::Value& json) {
        AddLogicalDriveDTO::Request add_request;
        add_request.set_type(json.is_member(TYPE_FIELD) ?
                            json[TYPE_FIELD].as_string() : TYPE_DEFAULT_VALUE);
        add_request.set_capacity_gb(json[CAPACITY_GB].as_uint());
        add_request.set_mode(json[MODE].as_string());
        add_request.set_snapshot(
                    json.is_member(SNAPSHOT) ? json[SNAPSHOT].as_bool() : false);
        add_request.set_protected(json[PROTECTED].as_bool());
        const auto& master_drive = logical_drives.get_node_by_id(
                                json[LINKS][MASTER_DRIVE][ODATA].as_string());
        add_request.set_master(master_drive.get_uuid());
        for(const auto& logical_drive : json[LINKS][LogicalDrives::TYPE].as_array()){
            const auto& drive_node = logical_drives.get_node_by_id(
                                            logical_drive[ODATA].as_string());
            add_request.add_drive(drive_node.get_uuid());
        }
        return add_request;
    }

    void link_storage_manager(const LogicalDrives& logical_drives,
                                Node& logical_drive) {
        try {
            const auto& managers = logical_drives.get_node_by_id(MANAGERS_PATH);
            const auto& manager = managers.get_next();
            if (nullptr == manager) {
                log_error(GET_LOGGER("rest"), "Unable to access: " << MANAGERS_PATH
                    <<" collection element");
                throw ServerException(ErrorFactory::create_invalid_payload_error());
            }
            logical_drive.add_link(Resource::MANAGED_BY, *manager, LogicalDrives::TYPE);
        } catch (const std::exception&) {
            log_error(GET_LOGGER("rest"), "Unable to set: " << Resource::MANAGED_BY
                    << " property for LogicalDrive");
        }
    }
}

void LogicalDrives::post(const Request& request, Response& response) {
    auto json = validate_request(*this, request);
    auto add_request =  create_request(*this, json);
    auto service = psme::core::service::ServiceFactory::create_storage(get_gami_id());
    auto add_response = service.add_logical_drive(add_request);

    if (!add_response.is_valid()) {
        throw ServerException(ErrorFactory::create_invalid_payload_error());
    }
    auto logical_drive = std::make_shared<LogicalDrive>(add_response.get_drive(),
                                           get_gami_id());
    logical_drive->get_resource().set_status({StateType::STARTING, HealthType::OK});
    logical_drive->get_resource().patch(json);

    link_storage_manager(*this, *logical_drive);

    add_node(logical_drive);
    add_link(Resource::MEMBERS, *logical_drive);

    std::string location = "http://"
        + request.get_headers().get_header_values(HttpHeaders::HOST).as_string()
        + logical_drive->get_path();

    response.add_header(HttpHeaders::LOCATION, location);
    response.set_reply(http::HttpStatusCode::CREATED);
}

void LogicalDrive::del(const Request&, Response& response) {
    // do not delete logical drive if any target defined
    const auto& lnks = get_links();
    if (std::end(lnks) != std::find_if(std::begin(lnks), std::end(lnks),
        [] (const Link& link) {return link.m_node->get_type() == Target::TYPE;}))
    {
        log_warning(GET_LOGGER("rest"), "Unable to remove logical drive: "
                << get_uuid() << ", target defined on this drive.");
        throw ServerException(ErrorFactory::create_invalid_payload_error());
    }

    // execute JSON-RPC command
    auto service = core::service::ServiceFactory::create_storage(get_gami_id());
    auto result = service.delete_logical_drive(get_uuid());
    if (result.is_valid()) {
        erase(*this);
        response.set_reply(http::HttpStatusCode::NO_CONTENT);
    } else {
        const auto& error = result.get_error();
        log_error(GET_LOGGER("rest"),  error.to_string());
        throw ServerException(ErrorFactory::create_invalid_payload_error());
    }
}

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<LogicalDrives> g_ldrives(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#Drive.1.0.0.LogicalDrivesCollection"},
        {Resource::NAME, "Logical Drives Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<LogicalDrive> g_ldrive(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#LogicalDrive.1.0.0.LogicalDrive"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Logical Drive"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {PROTECTED, json::Value::Type::BOOLEAN},
        {MODE, json::Value::Type::STRING},
        {TYPE_FIELD, json::Value::Type::STRING},
        {SNAPSHOT, json::Value::Type::BOOLEAN},
        {CAPACITY_GB, json::Value::Type::NUMBER},
        {ASSET_TAG, json::Value::Type::STRING},
        {IMAGE, json::Value::Type::STRING},
        {BOOTABLE, json::Value::Type::BOOLEAN},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair(Targets::TYPE, json::Value::Type::ARRAY),
                Pair(Resource::MANAGED_BY, json::Value::Type::ARRAY),
                Pair(USED_BY, json::Value::Type::ARRAY),
                Pair(PHYSICAL_DRIVES, json::Value::Type::ARRAY),
                Pair(LogicalDrives::TYPE, json::Value::Type::ARRAY),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })}
    }});
