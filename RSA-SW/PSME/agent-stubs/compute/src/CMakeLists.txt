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

add_subdirectory(command)
add_subdirectory(discovery)

add_library(compute-commands-stubs STATIC
    $<TARGET_OBJECTS:compute-command-stubs>
)

add_library(compute-ipmi-stubs
    $<TARGET_OBJECTS:discovery-impl-stubs>
)

add_executable(psme-compute-stubs
    main.cpp
)

target_link_libraries(psme-compute-stubs
    ${AGENT_COMPUTE_LIBRARIES}
    compute-ipmi-stubs
    ${AGENT_FRAMEWORK_LIBRARIES}
    ${UUID_LIBRARIES}
    ${LOGGER_LIBRARIES}
    ${PCA95XX_LIBRARIES}
    ${SAFESTRING_LIBRARIES}
    ${CONFIGURATION_LIBRARIES}
    ${JSONCXX_LIBRARIES}
    pthread
    jsonrpccpp-server
    jsonrpccpp-common
    jsonrpccpp-client
    jsoncpp
    ${CURL_LIBRARIES}
    ${MICROHTTPD_LIBRARIES}
)
