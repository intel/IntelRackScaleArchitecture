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
project("PSME Agent Framework" C CXX)

include(Version.cmake)

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/config/version.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/include/agent-framework/version.hpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

set(AGENT_COMMANDS_LIB -Wl,-whole-archive agent-commands -Wl,-no-whole-archive)
set(AGENT_FRAMEWORK_LIB agent-framework logger)
set(AGENT_FRAMEWORK_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

set(VALGRIND_SUPPRESSIONS
    ${CMAKE_CURRENT_SOURCE_DIR}/valgrind/jsonrpccpp.supp
)

set(CODE_COVERAGE_IGNORE
    '**/safe-string/*'
    '**/pca95xx/*'
    '**/json-cxx/*'
    '**/logger/*'
    '**/jsonrpccpp/*'
)

include(InstallSafeString)
include(InstallGoogleTest)
include(InstallGoogleMock)
include(InstallLogger)
include(InstallJsonCxx)
include(InstallJsonCpp)
include(InstallPCA95xx)
include(InstallCurl)
include(InstallMicrohttpd)
include(InstallJsonRpcCpp)
include(InstallUUID)
include(InstallConfiguration)

include(PsmeAgentFramework)
include(AddGnuCompiler)
include(AddClangCompiler)
include(AddCodeCoverage)
include(AddDiagnostics)
include(AddDocumentation)

find_package(SafeString)
find_package(Curl)
find_package(Microhttpd)
find_package(JsonCpp)
find_package(JsonRpcCpp)
find_package(GoogleTest COMPONENTS GoogleMock REQUIRED)
find_package(PCA95xx)
find_package(Logger)
find_package(JsonCxx)
find_package(UUID)
find_package(ValgrindCxx)

if (ENABLE_VALGRIND_DRD)
    include_directories(${VALGRINDCXX_INCLUDE_DIRS})
    add_compile_options(${VALGRINDCXX_COMPILE_FLAGS})
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

include_directories(SYSTEM
    ${UUID_INCLUDE_DIRS}
    ${PCA95XX_INCLUDE_DIRS}
    ${JSONCXX_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${JSONRPCCPP_INCLUDE_DIRS}
    ${LOGGER_INCLUDE_DIRS}
    ${SAFESTRING_INCLUDE_DIRS}
    ${CONFIGURATION_INCLUDE_DIRS}
)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
#add_subdirectory(examples)

install(DIRECTORY include/agent-framework DESTINATION include)
