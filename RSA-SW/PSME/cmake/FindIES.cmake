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

if (NOT IES_FOUND)
    find_file(ies_library fm_sdk.a
        PATHS ${CMAKE_LIBRARY_DIR}/lib
        PATH_SUFFIXES fulcrum focalpoint
   )

    find_library(ies_library fm_sdk)
    find_path(ies_include fm_sdk.h
        PATHS ${CMAKE_BINARY_DIR}/include
        PATH_SUFFIXES focalpoint alos
    )

    if (ies_library AND ies_include)
        add_library(fm_sdk IMPORTED SHARED)
        set_target_properties(fm_sdk PROPERTIES
            IMPORTED_LOCATION ${ies_library}
        )

        get_filename_component(IES_LIBRARY_DIRS ${ies_library}
            DIRECTORY
        )

        set(IES_LIBRARIES fm_sdk)
        set(IES_INCLUDE_DIRS ${ies_include};
            ${ies_include}/alos;
            ${ies_include}/alos/linux;
            ${ies_include}/std/intel;
            ${ies_include}/platforms;
            ${ies_include}/platforms/freedomTrail)
        set(IES_FOUND TRUE)
    else()
        message(WARNING "ies library or its headers were not found!")
    endif()
endif()
