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
    storage_services_info_dto.cpp
    drives_dto.cpp
    physical_drive_info_dto.cpp
    target_info_dto.cpp
    logical_drive_info_dto.cpp
    add_target_dto.cpp
    delete_target_dto.cpp
    add_logical_drive_dto.cpp
    delete_logical_drive_dto.cpp
)

add_library(core-dto-storage OBJECT ${SOURCES})
