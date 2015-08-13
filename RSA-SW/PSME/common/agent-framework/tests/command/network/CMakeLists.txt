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

if (NOT GTEST_FOUND)
    return()
endif()

add_gtest(network_command_test
    test_runner.cpp
    get_switch_info_test.cpp
    get_switch_port_info_test.cpp
    set_switch_port_attributes_test.cpp
    get_port_vlan_info_test.cpp
    get_module_info_test.cpp
    add_port_vlan_test.cpp
    delete_port_vlan_test.cpp
    get_components_test.cpp
    get_port_vlans_id_test.cpp
    get_remote_switch_info_test.cpp
    get_known_switches_id_test.cpp
    get_manager_info_test.cpp
    get_neighbor_switches_id_test.cpp
    get_switch_ports_id_test.cpp
)
target_link_libraries(network_command_test
    ${AGENT_COMMANDS_LIB}
    ${AGENT_FRAMEWORK_LIB}
    jsoncpp
    jsonrpccpp-server
    jsonrpccpp-common
    ${UUID_LIBRARIES}
    ${SAFESTRING_LIBRARIES}
    ${JSONCXX_LIBRARIES}
    ${PCA95XX_LIBRARIES}
)

target_include_directories(network_command_test PUBLIC
    ${AGENT_FRAMEWORK_DIR}/src/command/network
)
