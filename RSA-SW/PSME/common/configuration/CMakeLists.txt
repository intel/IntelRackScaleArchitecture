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
project(Configuration CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

include(AddGnuCompiler)
include(AddClangCompiler)
include(AddCodeCoverage)
include(AddDocumentation)

include(InstallSafeString)
include(InstallJsonCxx)
include(InstallLogger)
find_package(SafeString)
find_package(JsonCxx)
find_package(Logger)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(
    include
    ${JSONCXX_INCLUDE_DIRS}
    ${LOGGER_INCLUDE_DIRS}
    ${SAFESTRING_INCLUDE_DIRS}
)

add_subdirectory(src)

add_library(configuration STATIC
    src/configuration.cpp
    $<TARGET_OBJECTS:config-validator>
    $<TARGET_OBJECTS:config-validators>
)

install(TARGETS configuration
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/configuration DESTINATION include)
