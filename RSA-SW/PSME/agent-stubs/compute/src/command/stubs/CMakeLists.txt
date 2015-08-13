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

add_library(compute-command-stubs OBJECT
    initialization.cpp
    get_components.cpp
    get_manager_info.cpp
    get_memory_info.cpp
    get_drive_info.cpp
    get_processor_info.cpp
    get_module_info.cpp
    get_blade_info.cpp
    get_storage_controller_info.cpp
    get_network_interface_info.cpp
    set_blade_attributes.cpp
    get_chassis_info.cpp
    get_collection.cpp
    get_compute_info.cpp
    get_compute_zone_info.cpp
)

set_psme_command_target_properties(compute-command-stubs)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set_source_files_properties(
        get_manager_info.cpp
        PROPERTIES COMPILE_FLAGS "-Wno-missing-field-initializers"
    )
endif()
