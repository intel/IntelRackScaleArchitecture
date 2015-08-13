/*!
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
 *
 * @file resource.hpp
 *
 * @brief Declaration of Resource class.
 * */
#ifndef PSME_REST_RESOURCE_RESOURCE_HPP
#define	PSME_REST_RESOURCE_RESOURCE_HPP

#include "psme/rest/resource/resource_definition.hpp"
#include "psme/rest/resource/status.hpp"
#include "psme/rest/resource/location.hpp"
#include "json/json.hpp"
#include "logger_ext.hpp"

#include <string>
#include <memory>

namespace psme {
namespace rest {
namespace node {
    class Node;
}
}
}

namespace psme {
namespace rest {
namespace resource {

using std::string;
using psme::rest::node::Node;

/*!
 * @brief Class holding JSON representation of Node.
 *
 * */
class Resource {
public:
    /*! @brief Odata.context property */
    static constexpr char ODATA_CONTEXT[] = "@odata.context";
    /*! @brief Odata.id property */
    static constexpr char ODATA_ID[] = "@odata.id";
    /*! @brief Odata.type property */
    static constexpr char ODATA_TYPE[] = "@odata.type";
    /*! @brief Description property */
    static constexpr char DESCRIPTION[] = "Description";
    /*! @brief The date-time stamp that the object was last modified */
    static constexpr char MODIFIED[] = "Modified";
    /*! @brief Resource name */
    static constexpr char NAME[] = "Name";
    /*! @brief Links property */
    static constexpr char LINKS[] = "Links";
    /*! @brief Members property */
    static constexpr char MEMBERS[] = "Members";
    /*! @brief Members odata.count property */
    static constexpr char MEMBERS_ODATA_COUNT[] = "Members@odata.count";
    /*! @brief Members odata.nextLink property */
    static constexpr char MEMBERS_NEXT_LINK[] = "Members@odata.nextLink";
    /*! @brief Resource identifier */
    static constexpr char ID[] = "Id";
    /*! @brief OEM defined object */
    static constexpr char OEM[] = "Oem";
    /*! @brief Status of resource enumeration */
    static constexpr char ENUMERATED[] = "EnumStatus";
    /*! @brief Actions property */
    static constexpr char ACTIONS[] = "Actions";
    /*! @brief Action target property */
    static constexpr char ACTION_TARGET[] = "target";
    /*! @brief ManagedBy property */
    static constexpr char MANAGED_BY[] = "ManagedBy";
    /*! @brief ContainedBy property */
    static constexpr char CONTAINED_BY[] = "ContainedBy";
    /*! @brief Chassis property */
    static constexpr char CHASSIS[] = "Chassis";

    /*!
     * @brief Constructor
     *
     * @param type Type of resource
     * */
    Resource(const char* type);

    /*! @brief Default copy constructor */
    Resource(const Resource& orig) = default;

    /*! @brief Assignment operator */
    Resource& operator=(const Resource&) = default;

    /*!
     * @brief Sets resource location property.
     *
     * @param[in] location Location object.
     * */
    void set_location(const Location& location);

    /*!
     * @brief Sets resource status property.
     *
     * @param[in] status Status object.
     * */
    void set_status(const Status& status);

    /*!
     * @brief Sets resource enum status property.
     *
     * @param[in] enum_status EnumStatus enumeration.
     * */
    void set_enumerated(EnumStatus enum_status);

    /*!
     * @brief Resource property setter.
     *
     * @param[in] key Property key.
     * @param[in] value Property value.
     * */
    template<typename T>
    void set_property(const std::string& key, const T& value) {
        auto idx = find_property_idx(key);
        if (idx != npos) {
            const auto& property = m_resource_def.m_property_vec[idx];
            json::Value json(value);
            if (is_valid(property, json)) {
                m_json[key] = std::move(json);
            } else {
                log_warning(GET_LOGGER("rest"), " Invalid property: \""
                        << key << "\":" << value);
            }
        } else {
            log_warning(GET_LOGGER("rest"), " Undefined property: \""
                    << key << "\":" << value);
        }
    }

    /*!
     * @brief Updates node's odata.id property.
     *
     * @param[in] node Node to be updated
     * */
    void update_ids(const Node& node);

    /*!
     * @brief Updates node's Location property.
     *
     * @param[in] node Node to be updated
     * */
    void update_location(const Node& node);

    /*!
     * @brief Updates node's Links property.
     *
     * @param[in] node Node to be updated
     * */
    void update_links(const Node& node);

    /*!
     * @brief Updates node's Actions property.
     *
     * @param node Node to be updated
     * */
    void update_actions(Node& node);

    /*! @brief Updates node's Modified property. */
    void update_modified();

    /*!
     * @brief Updates node's Target Addresses Drive property.
     *
     * @param node Node to be updated?
     * */
    void update_target_drive_path(Node& node);

    /*!
     * @brief Updates node's JSON properties.
     *
     * @param node Node to be updated
     * */
    void update_json_properties(Node& node);

    /*!
     * @brief Patches resource object.
     *
     * @param[in] json JSON object to patch current resource.
     *
     * @return Resource JSON object.
     * */
    const json::Value& patch(const json::Value& json);

    /*!
     * @brief Gets resource JSON object.
     *
     * @return Resource JSON object.
     * */
    const json::Value& as_json() const { return m_json; }

    /*!
     * @brief Converts resource JSON value to string
     *
     * @return JSON value as a string
     * */
    string as_string() { return json::Serializer(m_json); }

private:
    json::Value m_json;
    bool m_update_cache;
    const ResourceDef& m_resource_def;

private:
    static const size_t npos = static_cast<size_t>(-1);
    size_t find_property_idx(const std::string& key) const;
    bool is_valid(const Property& property, const json::Value& value) const;
};

using ResourceUPtr = std::unique_ptr<Resource>;

}
}
}

#endif	/* PSME_REST_RESOURCE_RESOURCE_HPP */

