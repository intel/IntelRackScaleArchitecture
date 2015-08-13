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
    processor_info_dto.cpp
    memory_info_dto.cpp
    drive_info_dto.cpp
    storage_controller_info_dto.cpp
    network_interface_info_dto.cpp
    module_info_dto.cpp
    blade_info_dto.cpp
    blade_attributes_dto.cpp
    compute_info_dto.cpp
    compute_zone_info_dto.cpp
    chassis_info_dto.cpp
)

add_library(core-dto-compute OBJECT ${SOURCES})
