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
project("PSME FW Build" C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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
include(InstallPopt)
include(InstallNcurses)
include(InstallOpenIPMI OPTIONAL)
include(InstallSysfs OPTIONAL)
include(InstallLvm2App OPTIONAL)
include(InstallAgentFramework)
include(InstallIES OPTIONAL)

include(AddGnuCompiler)
include(AddClangCompiler)
include(AddCodeCoverage)
include(AddDocumentation)
include(AddCPack)

enable_testing()

find_package(SafeString)
find_package(GoogleTest COMPONENTS GoogleMock REQUIRED)
find_package(Logger)
find_package(PCA95xx)
find_package(JsonCxx)
find_package(JsonCpp)
find_package(JsonRpcCpp)
find_package(Popt)
find_package(Curl)
find_package(Microhttpd)
find_package(OpenIPMI)
find_package(UUID)
find_package(Sysfs)
find_package(Lvm2App)
find_package(AgentFramework)
find_package(IES)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/agent-stubs)
    add_subdirectory(agent-stubs)
endif()
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/agent)
    add_subdirectory(agent)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/agent-intel)
    add_subdirectory(agent-intel)
endif()

add_subdirectory(application)
add_subdirectory(encrypter)
