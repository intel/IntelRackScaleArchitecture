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
    chassis_type.cpp
    resource.cpp
    resource_utils.cpp
    status.cpp
    system_type.cpp
    location.cpp
    resource_definition.cpp
)

add_library(rest-resource OBJECT ${SOURCES})

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set_source_files_properties(
        resource.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-useless-cast"
    )
if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set_source_files_properties(
        link_resolver.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-unsafe-loop-optimizations"
    )
endif()

endif()
