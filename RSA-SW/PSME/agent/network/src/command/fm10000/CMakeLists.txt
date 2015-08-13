# <license_header>
#
# Copyright (c) 2015 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# </license_header>

set(SOURCES
    initialization.cpp
    get_components.cpp
    get_manager_info.cpp
    get_module_info.cpp
    get_switch_info.cpp
    get_switch_ports_id.cpp
    get_switch_port_info.cpp
    set_switch_port_attributes.cpp
    get_port_vlan_info.cpp
    get_port_vlans_id.cpp
    add_port_vlan.cpp
    delete_port_vlan.cpp
    get_remote_switch_info.cpp
    get_neighbor_switches_id.cpp
    get_known_switches_id.cpp
)

add_library(network-command-fm10000 OBJECT ${SOURCES})
set_psme_command_target_properties(network-command-fm10000)
