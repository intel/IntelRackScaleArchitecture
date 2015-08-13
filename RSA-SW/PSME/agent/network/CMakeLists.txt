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

cmake_minimum_required(VERSION 2.8)
project("PSME Network" C CXX)

if (CMAKE_CROSSCOMPILING)
  message(WARNING "Not supported for crosscompiling")
  return()
endif()

set(VALGRIND_SUPPRESSIONS
    ${CMAKE_CURRENT_SOURCE_DIR}/valgrind/jsonrpccpp.supp
)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
set(AGENT_NETWORK_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(InstallSafeString)
include(InstallGoogleTest)
include(InstallGoogleMock)
include(InstallLogger)
include(InstallPCA95xx)
include(InstallJsonCxx)
include(InstallJsonCpp)
include(InstallCurl)
include(InstallMicrohttpd)
include(InstallJsonRpcCpp)
include(InstallUUID)
include(InstallAgentFramework)
include(InstallIES)
include(InstallConfiguration)

include(PsmeAgentFramework)
include(AddGnuCompiler)
include(AddClangCompiler)
include(AddCodeCoverage)
include(AddDocumentation)

enable_testing()

find_package(SafeString)
find_package(GoogleTest COMPONENTS GoogleMock REQUIRED)
find_package(Logger)
find_package(PCA95xx)
find_package(JsonCxx)
find_package(JsonCpp)
find_package(JsonRpcCpp)
find_package(Curl)
find_package(Microhttpd)
find_package(UUID)
find_package(AgentFramework)
find_package(SafeString)
find_package(NL3)
find_package(UDEV)
find_package(IES)

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/config/network_config.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/src/network_config.hpp
)

include_directories(
    ${AGENT_FRAMEWORK_INCLUDE_DIRS}
    src
    include
)

include_directories(SYSTEM
    ${UUID_INCLUDE_DIRS}
    ${LOGGER_INCLUDE_DIRS}
    ${SAFESTRING_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${JSONCXX_INCLUDE_DIRS}
    ${JSONRPCCPP_INCLUDE_DIRS}
    ${NL3_INCLUDE_DIRS}
    ${IES_INCLUDE_DIRS}
    ${CONFIGURATION_INCLUDE_DIRS}
)

link_libraries(${NL3_LIBRARIES})
link_libraries(${UDEV_LIBRARIES})

add_subdirectory(src)
add_subdirectory(tests)
