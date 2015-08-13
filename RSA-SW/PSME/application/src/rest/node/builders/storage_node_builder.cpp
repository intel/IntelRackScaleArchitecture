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

#include "psme/rest/node/builders/storage_node_builder.hpp"
#include "psme/rest/node/builders/builder_util.hpp"
#include "psme/rest/node/crud/services.hpp"
#include "psme/rest/node/crud/drives.hpp"
#include "psme/rest/node/crud/physical_drive.hpp"
#include "psme/rest/node/crud/logical_drives.hpp"
#include "psme/rest/node/crud/targets.hpp"
#include "psme/rest/node/crud/storage_managers.hpp"
#include "psme/rest/node/crud/managers.hpp"
#include "psme/rest/resource/resource.hpp"
#include "core/service/service_factory.hpp"
#include "core/service/storage_service.hpp"

using namespace psme::rest::node;
using namespace psme::rest::resource;
using namespace psme::rest::utils;
using namespace psme::core::dto;

using psme::core::service::ServiceFactory;
using psme::core::service::AgentService;

StorageNodeBuilder::~StorageNodeBuilder() { }

namespace {
    constexpr const char PHYSICAL_DRIVES_COLLECTION_TYPE[] = "PhysicalDrives";
    constexpr const char TARGETS_COLLECTION_TYPE[] = "iSCSITargets";
    constexpr const char LOGICAL_DRIVES_COLLECTION_TYPE[] = "LogicalDrives";
}

NodesLinkVec
StorageNodeBuilder::build_nodes(Node& root, const string& component_uuid) {

    auto storage_service = ServiceFactory::create_storage(get_agent()->get_gami_id());

    m_root = &root;
    if (nullptr == root.get_next() || nullptr == root.get_next()->get_next()) {
        throw std::runtime_error("Tree is not properly initialized.");
    }
    auto* v_node = root.get_next()->get_next();
    auto& services_node = v_node->get_node_by_id(Services::TYPE);
    // only one storage manager node which is already created

    auto* storage_manager_node = v_node->get_node_by_id(Managers::TYPE).get_next();
    if (nullptr == storage_manager_node) {
        throw std::runtime_error("Tree is not properly initialized.");
    }

    NodesLinkVec nodes_to_link;

    auto service_node = create_service(component_uuid);

    nodes_to_link.emplace_back(LinkType::COMPOSITION,
            Resource::MEMBERS, services_node, service_node);
    nodes_to_link.emplace_back(LinkType::ASSOCIATION, Services::TYPE,
            *storage_manager_node, service_node, Resource::MANAGED_BY);

    // service node specific begin
    const auto& gami_id = get_agent()->get_gami_id();
    auto drives_node = std::make_shared<Drives>("", gami_id);
    NodeBuilder::link_nodes(LinkType::COMPOSITION,
            Drives::TYPE, *service_node, drives_node);
    auto targets_node = std::make_shared<Targets>("", gami_id);
    NodeBuilder::link_nodes(LinkType::COMPOSITION,
            Targets::REMOTE_TARGETS, *service_node, targets_node);
    auto logical_drives_node = std::make_shared<LogicalDrives>("", gami_id);
    NodeBuilder::link_nodes(LinkType::COMPOSITION,
            LogicalDrives::TYPE, *service_node, logical_drives_node);
    // service node specific end

    auto storage_services_info =
        storage_service.get_storage_services_info(component_uuid);
    const auto& collections = storage_services_info.get_collections();
    std::map<string, string> collections_by_type{};
    for (const auto& collection : collections.get_collections()) {
        collections_by_type[collection.get_type()] = collection.get_name();
    }

    if (collections_by_type.end() !=
        collections_by_type.find(PHYSICAL_DRIVES_COLLECTION_TYPE)) {
        build_drives(storage_service, *drives_node, component_uuid,
                     collections_by_type[PHYSICAL_DRIVES_COLLECTION_TYPE]);

        for (auto& drive : *drives_node) {
            drive.add_link(Resource::CONTAINED_BY, *service_node);
        }
    }

    if (collections_by_type.end() !=
        collections_by_type.find(LOGICAL_DRIVES_COLLECTION_TYPE)) {
        auto delayed_links = build_logical_drives(storage_service,
                        *logical_drives_node, *storage_manager_node, component_uuid,
                        collections_by_type[LOGICAL_DRIVES_COLLECTION_TYPE]);

        for (const auto& link: delayed_links) {
            nodes_to_link.push_back(link);
        }
    }

    if (collections_by_type.end() !=
        collections_by_type.find(TARGETS_COLLECTION_TYPE)) {
        auto delayed_links = build_targets(storage_service, *targets_node,
                *storage_manager_node, *logical_drives_node, component_uuid,
                collections_by_type[TARGETS_COLLECTION_TYPE]);

        for (const auto& link: delayed_links) {
            nodes_to_link.push_back(link);
        }
    }

    resolve_dependencies(*logical_drives_node, *drives_node, *targets_node, storage_service);

    return nodes_to_link;
}

void StorageNodeBuilder::resolve_dependencies(Node& logical_drives_node, Node& drives_node, Node& targets_node, StorageService& storage_service) {
    for (auto& logical_drive : logical_drives_node) {
        auto children = find_children(logical_drive, LOGICAL_DRIVES_COLLECTION_TYPE, storage_service, logical_drives_node);
        auto physical_drives_children = find_children(logical_drive, PHYSICAL_DRIVES_COLLECTION_TYPE, storage_service, drives_node);
        for (auto* child : children) {
            logical_drive.add_link(LOGICAL_DRIVES_COLLECTION_TYPE, *child, "UsedBy");
        }

        for (auto* child : physical_drives_children) {
            logical_drive.add_link(PHYSICAL_DRIVES_COLLECTION_TYPE, *child, "UsedBy");
        }
    }

    for (auto& target : targets_node) {
        auto children = find_children(target, LOGICAL_DRIVES_COLLECTION_TYPE, storage_service, logical_drives_node);
        for (auto* child : children) {
            target.add_link(LOGICAL_DRIVES_COLLECTION_TYPE, *child, "Targets");
        }
    }
}

std::list<Node*>
StorageNodeBuilder::find_children(const Node& parent,
        const std::string& collection_name,
        StorageService& storage_service, const Node& root_node) {

    std::list<Node*> children;

    std::string parent_uuid = parent.get_uuid();
    auto subcomponents = storage_service.get_collection(parent_uuid, collection_name);

    for (const auto& subcomponent : subcomponents) {
        auto child_uuid = subcomponent.get_subcomponent();
        auto* child = root_node.get_node_by_uuid(child_uuid);
        if (child != nullptr) {
            children.push_back(child);
        }
    }

    return children;
}

NodesLinkVec
StorageNodeBuilder::build_logical_drives(StorageService& storage_service,
                    Node& parent,
                    Node& storage_manager_node,
                    const std::string& component_uuid,
                    const std::string& collection_name) {

    auto response =
        storage_service.get_collection(component_uuid, collection_name);

    NodesLinkVec nodes_to_link;
    for (const auto& subcomponent : response) {
        const auto& logical_drive_uuid = subcomponent.get_subcomponent();
        auto logical_drive_info = storage_service.get_logical_drive_info(logical_drive_uuid);

        auto logical_drive_node = std::make_shared<LogicalDrive>(
                                logical_drive_uuid, get_agent()->get_gami_id(),
                                get_node_id(logical_drive_uuid));

        NodeBuilder::link_nodes(LinkType::COMPOSITION,
                Resource::MEMBERS, parent, logical_drive_node);
        nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                    LogicalDrives::TYPE, storage_manager_node,
                    logical_drive_node, Resource::MANAGED_BY);

        json::Value json_content;
        json_content[Status::STATUS] = to_resource_status(logical_drive_info.get_status()).as_json();
        json_content["CapacityGB"] = logical_drive_info.get_capacity_gb();
        json_content["Mode"] = logical_drive_info.get_mode();
        json_content["Type"] = logical_drive_info.get_type();
        json_content["Bootable"] = logical_drive_info.get_bootable();
        json_content["Protected"] = logical_drive_info.get_protected();
        json_content["Snapshot"] = logical_drive_info.get_snapshot();
        json_content["Image"] = logical_drive_info.get_image();

        logical_drive_node->get_resource().patch(json_content);
    }
    return nodes_to_link;
}

void StorageNodeBuilder::build_drives(StorageService& storage_service,
                                      Node& parent,
                                      const std::string& component_uuid,
                                      const std::string& collection_name) {
    auto response =
        storage_service.get_collection(component_uuid, collection_name);

    for (const auto& subcomponent : response) {
        const std::string& disk_uuid = subcomponent.get_subcomponent();
        auto drive_info = storage_service.get_physical_drive_info(disk_uuid);
        auto drive_node = std::make_shared<PhysicalDrive>(disk_uuid,
                get_agent()->get_gami_id(), get_node_id(disk_uuid));

        NodeBuilder::link_nodes(LinkType::COMPOSITION,
                Resource::MEMBERS, parent, drive_node);

        json::Value json_content;
        json_content[Status::STATUS] = to_resource_status(drive_info.get_status()).as_json();
        json_content["Interface"] = drive_info.get_interface();
        json_content["CapacityGB"] = drive_info.get_capacity_gb();
        json_content["RPM"] = drive_info.get_rpm();
        json_content["Model"] = drive_info.get_fru_info().get_model_number();
        json_content["SerialNumber"] = drive_info.get_fru_info().get_serial_number();
        json_content["Manufacturer"] = drive_info.get_fru_info().get_manufacturer();
        json_content["Type"] = drive_info.get_type();

        drive_node->get_resource().patch(json_content);
    }
}

NodeSharedPtr
StorageNodeBuilder::create_service(const std::string& component_uuid) {
    auto service = std::make_shared<Service>(component_uuid,
            get_agent()->get_gami_id(), get_node_id(component_uuid));

    // populate JSON data
    json::Value json_content;
    Status status(StateType::ENABLED, HealthType::OK);
    json_content[Status::STATUS] = status.as_json();
    json_content[Resource::ENUMERATED] = to_string(EnumStatus::ENUMERATED);
    json_content["Location"] = json::Value(json::Value::Type::OBJECT);

    service->get_resource().patch(json_content);

    return service;
}

NodesLinkVec
StorageNodeBuilder::build_targets(StorageService& service,
                                  Node& parent,
                                  Node& storage_manager_node,
                                  const Node& logical_drives,
                                  const std::string& component_uuid,
                                  const std::string& collection_name) {
    const auto response =
        service.get_collection(component_uuid, collection_name);

    NodesLinkVec nodes_to_link;
    for (const auto& subcomponent : response) {
        auto target_info =
                    service.get_target_info(subcomponent.get_subcomponent());
        auto target = std::make_shared<Target>(subcomponent.get_subcomponent(),
                                get_agent()->get_gami_id(),
                                get_node_id(subcomponent.get_subcomponent()));
        link_nodes(LinkType::COMPOSITION,
                   Resource::MEMBERS, parent, target);
        nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                    Targets::REMOTE_TARGETS, storage_manager_node,
                    target, Resource::MANAGED_BY);

        json::Value json;
        json[Status::STATUS] = to_resource_status(
                                        target_info.get_status()).as_json();

        json[Resource::ENUMERATED] = to_string(EnumStatus::ENUMERATED);
        json["Type"] = TARGETS_COLLECTION_TYPE;

        json::Value luns(json::Value::Type::ARRAY);
        for (const auto& lun : target_info.get_target_luns()) {
            if (auto logical_drive = logical_drives.get_node_by_uuid(
                                            lun.get_logical_drive_uuid())) {
                json::Value lun_json;
                lun_json["LUN"] = static_cast<unsigned>(lun.get_lun());
                lun_json["Drive"] = logical_drive->get_path();
                luns.push_back(std::move(lun_json));
            }
        }

        json::Value addr;
        addr["TargetIQN"] = target_info.get_target_iqn();
        addr["TargetLUN"] = luns;
        addr["TargetPortalIP"] = target_info.get_target_address();
        addr["TargetPortalPort"]  = target_info.get_target_port();
        addr[Status::STATUS] = to_resource_status(
                                        target_info.get_status()).as_json();
        json::Value enum_iscsi;
        enum_iscsi["iSCSI"] = addr;
        json["Addresses"].push_back(enum_iscsi);
        json::Value initiator;
        initiator["iSCSI"]["InitiatorIQN"] = target_info.get_initiator_iqn();
        json["Initiator"].push_back(initiator);

        target->get_resource().patch(json);
    }
    return nodes_to_link;
}

std::string
StorageNodeBuilder::get_node_id(const std::string& uuid) {
    if (nullptr != m_root) {
        auto* found = m_root->get_node_by_uuid(uuid);
        if (nullptr != found) {
            return found->get_id();
        }
    }
    return "";
}
