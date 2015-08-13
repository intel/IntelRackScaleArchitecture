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
 * @file resource_definition.hpp
 *
 * @brief Declaration of ResourceDefinitions class
 * */

#ifndef PSME_REST_RESOURCE_RESOURCE_DEFINITION_HPP
#define PSME_REST_RESOURCE_RESOURCE_DEFINITION_HPP

#include "json/value.hpp"
#include <string>
#include <vector>
#include <map>

namespace psme {
namespace rest {
namespace resource {

using std::string;

/*!
 * @brief Resource JSON Property Definition.
 *
 * */
struct Property {
    /*!
     * @brief Property definition constructor
     *
     * @param[in] name The name of property
     * @param default_value Default value for property
     * @param required Property requirement
     * @param readonly Sets property to read-only
     * */
    Property(const string& name,
            json::Value default_value = json::Value::Type::NIL,
            bool required=false, bool readonly=false)
        : m_name(name), m_default_value(default_value),
          m_required(required), m_readonly(readonly) { }

    /*! @brief Property name */
    string m_name;
    /*! @brief Default property value */
    json::Value m_default_value;
    /*! @brief Is property required */
    bool m_required;
    /*! @brief Is property read-only */
    bool m_readonly;
};

using PropertyVec = std::vector<Property>;

/*!
 * @brief Structure describing Resource.
 *
 * Contains information about resource properties
 * */
struct ResourceDef {
    /*!
     * @brief Resource definition constructor
     *
     * @param[in] property_vec Collection of properties
     * */
    ResourceDef(const PropertyVec& property_vec = {})
        :  m_property_vec(property_vec) { }

    /*! @brief Resource JSON property definitions */
    PropertyVec m_property_vec;
};

using ResourceDefMap = std::map<string, ResourceDef>;

/*!
 * @brief Singleton pattern. Provides access to resource definitions.
 */
class ResourceDefinitions {
public:
    /*! @brief Registers resource definitions in a map */
    template<class T>
    class Register {
    public:
        /*!
         * @brief Constructor
         *
         * @param[in] resource_def Resource definition object
         * */
        Register(const ResourceDef& resource_def) {
            ResourceDefinitions::add(T::TYPE, resource_def);
        }
    };

    /*!
     * @brief Singleton pattern. Returns instance of ResourceDefinitions
     *
     * @return Instance of ResourceDefinitions.
     * */
    static ResourceDefinitions& get_instance();

    /*!
     * @brief Returns resource definition
     *
     * @param type Type of resource
     *
     * @return Resource definition object
     * */
    static const ResourceDef& get_resource_def(const char* type);

    /*!
     * @brief Adds resource definition
     *
     * @param[in] type Type of resource
     * @param[in] resource_def Resource definition object
     * */
    static void add(const string& type, const ResourceDef& resource_def);

    template<class T> friend class Register;
private:
    static constexpr const char UNDEFINED[] = "UNDEFINED";
    /*! @brief Default constructor */
    ResourceDefinitions();
    ResourceDefinitions(const ResourceDefinitions&) = delete;
    ResourceDefinitions& operator=(const ResourceDefinitions&) = delete;

    /*! @brief Resource definitions map */
    ResourceDefMap m_resource_map;
};

}
}
}

#endif /* PSME_REST_RESOURCE_RESOURCE_DEFINITION_HPP */
