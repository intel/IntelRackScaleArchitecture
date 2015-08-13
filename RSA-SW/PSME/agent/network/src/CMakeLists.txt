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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(api)
add_subdirectory(hw)
add_subdirectory(command)
add_subdirectory(discovery)

set(network_commands_targets
    $<TARGET_OBJECTS:network-command-fm6000>
    $<TARGET_OBJECTS:network-command-fm10000>
)

if (network_commands_targets)
    add_library(network-commands STATIC ${network_commands_targets})

    set(AGENT_NETWORK_LIBRARIES
        -Wl,--whole-archive network-commands -Wl,--no-whole-archive
    )
endif()

add_library(network-hw-api
    $<TARGET_OBJECTS:api>
    $<TARGET_OBJECTS:hw>
)

add_library(discovery-network
    $<TARGET_OBJECTS:discovery-impl-network>
)

add_executable(psme-network
    main.cpp
)

target_link_libraries(psme-network
    ${AGENT_NETWORK_LIBRARIES}
    network-hw-api
    discovery-network
    ${AGENT_FRAMEWORK_LIBRARIES}
    ${UUID_LIBRARIES}
    ${LOGGER_LIBRARIES}
    ${CONFIGURATION_LIBRARIES}
    ${JSONCXX_LIBRARIES}
    ${PCA95XX_LIBRARIES}
    ${SAFESTRING_LIBRARIES}
    ${IES_LIBRARIES}
    pthread
    jsonrpccpp-server
    jsonrpccpp-common
    jsonrpccpp-client
    jsoncpp
    ${MICROHTTPD_LIBRARIES}
    ${CURL_LIBRARIES}
    dl
 )
