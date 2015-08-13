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
    request_dto.cpp
    response_dto.cpp
    status_dto.cpp
    fru_info_dto.cpp
    oem_data_dto.cpp
    ipv4_address_dto.cpp
    ipv6_address_dto.cpp
    neighbor_info_dto.cpp
    component_dto.cpp
    components_dto.cpp
    component_collection_dto.cpp
    manager_info_dto.cpp
    collections_dto.cpp
    collection_dto.cpp
    attribute_dto.cpp
    set_component_attributes_dto.cpp
    error.cpp
)

add_library(core-dto OBJECT ${SOURCES})

add_subdirectory(compute)
add_subdirectory(network)
add_subdirectory(storage)
