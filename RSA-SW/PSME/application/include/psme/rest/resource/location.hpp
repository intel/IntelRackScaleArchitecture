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
 * @file location.hpp
 *
 * @brief Declaration of Location class
 * */

#ifndef PSME_REST_RESOURCE_LOCATION_HPP
#define PSME_REST_RESOURCE_LOCATION_HPP

#include "json/json.hpp"


namespace psme {
namespace rest {
namespace node {
    class Node;
}
}
}

/*! Psme namespace */
namespace psme {
/*! Rest namespace */
namespace rest {
/*! Resource namespace */
namespace resource {

using std::string;
using psme::rest::node::Node;

/*!
 * @brief Class holding JSON representation of Location.
 *
 * */
class Location {
public:
    /*! @brief Resource location */
    static constexpr char LOCATION[] = "Location";
    /*! @brief Rack location */
    static constexpr char RACK[] = "Rack";
    /*! @brief Pod location */
    static constexpr char POD[] = "Pod";
    /*! @brief Drawer location */
    static constexpr char DRAWER[] = "Drawer";
    /*! @brief Module location */
    static constexpr char MODULE[] = "Module";
    /*! @brief Blade location */
    static constexpr char BLADE[] = "Blade";
    /*! @brief Switch location */
    static constexpr char SWITCH[] = "Switch";

    /*! @brief Constructor */
    Location();

    /*!
     * @brief Resolves location of given node.
     *
     * @param node Node for which we resolve location
     * @param location holds partially resolved location (up to given node).
     *
     * @return Location of given node.
     * */
    static Location& resolve(const Node& node, Location& location);

    /*!
     * @brief Sets rack location
     * @param rack Location of rack
     */
    void set_rack(const int rack) {
        m_json[RACK] = rack;
    }

    /*!
     * @brief Sets pod location
     * @param pod Location of pod
     */
    void set_pod(const int pod) {
        m_json[POD] = pod;
    }

    /*!
     * @brief Sets drawer location
     * @param drawer Location of drawer
     */
    void set_drawer(const int drawer) {
        m_json[DRAWER] = drawer;
    }

    /*!
     * @brief Sets module location
     * @param module Location of module
     */
    void set_module(const int module) {
        m_json[MODULE] = module;
    }

    /*!
     * @brief Sets blade location
     * @param blade Location of blade
     */
    void set_blade(const int blade) {
        m_json[BLADE] = blade;
    }

    /*!
     * @brief Sets switch location
     * @param switch_no Location of switch
     */
    void set_switch(const int switch_no) {
        m_json[SWITCH] = switch_no;
    }

    /*!
     * @brief Converts location to JSON
     * @return Resource location as JSON
     */
    const json::Value& as_json() const { return m_json; }
private:
    json::Value m_json;
};

} /*! Resource namespace */
} /*! Rest namespace */
} /*! Psme namespace */

#endif	/* PSME_REST_RESOURCE_LOCATION_HPP */
