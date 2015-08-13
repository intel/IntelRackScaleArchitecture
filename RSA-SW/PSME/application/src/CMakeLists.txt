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

set (SOURCES
    version.cpp
    logger_ext.cpp
    logger_loader.cpp
    signal.cpp
    main.cpp
)

include_directories(
    ${PSME_APPLICATION_DIR}/src/registration
    ${PSME_APPLICATION_DIR}/src/eventing
    ${PSME_APPLICATION_DIR}/src/command
    ${PSME_APPLICATION_DIR}/src/core
)

add_subdirectory(command)
add_subdirectory(eventing)
add_subdirectory(registration)
add_subdirectory(core)
add_subdirectory(rest)
add_subdirectory(utils)

add_library(application
    $<TARGET_OBJECTS:core-agent>
    $<TARGET_OBJECTS:core-dto>
    $<TARGET_OBJECTS:core-dto-compute>
    $<TARGET_OBJECTS:core-dto-storage>
    $<TARGET_OBJECTS:core-dto-network>
    $<TARGET_OBJECTS:core-service>
    $<TARGET_OBJECTS:command-server>
    $<TARGET_OBJECTS:command-registration>
    $<TARGET_OBJECTS:command-heart-beat>
    $<TARGET_OBJECTS:command-eventing>
    $<TARGET_OBJECTS:registration-server>
    $<TARGET_OBJECTS:eventing-server>
    $<TARGET_OBJECTS:app-utils>
)

add_library(application-commands
    $<TARGET_OBJECTS:command-heart-beat-json>
    $<TARGET_OBJECTS:command-heart-beat-definition>
    $<TARGET_OBJECTS:command-registration-json>
    $<TARGET_OBJECTS:command-registration-definition>
    $<TARGET_OBJECTS:command-eventing-json>
    $<TARGET_OBJECTS:command-eventing-definition>
)

add_library(application-rest
    $<TARGET_OBJECTS:rest-node>
    $<TARGET_OBJECTS:rest-node-builders>
    $<TARGET_OBJECTS:rest-node-crud>
    $<TARGET_OBJECTS:rest-error>
    $<TARGET_OBJECTS:rest-resource>
    $<TARGET_OBJECTS:rest-http>
)

add_executable(psme-rest-server ${SOURCES})

target_link_libraries(psme-rest-server
    ${LOGGER_LIBRARIES}
    -Wl,--whole-archive application-commands -Wl,--no-whole-archive
    application-rest
    application
    ${UUID_LIBRARIES}
    ${MICROHTTPD_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${JSONRPCCPP_LIBRARIES}
    ${SAFESTRING_LIBRARIES}
    ${CONFIGURATION_LIBRARIES}
    ${JSONCXX_LIBRARIES}
    ${CURL_LIBRARIES}
)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU|Clang)
    set_source_files_properties(
        signal.cpp
        PROPERTIES
        COMPILE_FLAGS "-Wno-old-style-cast"
    )
endif()
