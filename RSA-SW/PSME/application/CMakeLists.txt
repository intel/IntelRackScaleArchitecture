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
project("PSME Application" C CXX)

include(Version.cmake)

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/config/version.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/include/version.hpp
)

set(PSME_APPLICATION_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(VALGRIND_SUPPRESSIONS
    ${CMAKE_CURRENT_SOURCE_DIR}/valgrind/jsonrpccpp.supp
)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

include(InstallSafeString)
include(InstallGoogleTest)
include(InstallGoogleMock)
include(InstallLogger)
include(InstallJsonCxx)
include(InstallJsonCpp)
include(InstallCurl)
include(InstallMicrohttpd)
include(InstallJsonRpcCpp)
include(InstallUUID)
include(InstallConfiguration)

include(Application)
include(AddGnuCompiler)
include(AddClangCompiler)
include(AddCodeCoverage)
include(AddDocumentation)

enable_testing()

find_package(SafeString)
find_package(UUID)
find_package(Logger)
find_package(JsonCpp)
find_package(JsonCxx)
find_package(JsonRpcCpp)
find_package(Curl)
find_package(Microhttpd)
find_package(GoogleTest COMPONENTS GoogleMock REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
include_directories(${SAFESTRING_INCLUDE_DIRS})

include_directories(SYSTEM
    ${UUID_INCLUDE_DIRS}
    ${LOGGER_INCLUDE_DIRS}
    ${JSONCXX_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${JSONRPCCPP_INCLUDE_DIRS}
    ${CONFIGURATION_INCLUDE_DIRS}
)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)
