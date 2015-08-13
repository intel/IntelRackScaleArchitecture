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

if (NOT LVM2APP_FOUND)
    find_file(lvm2app_library liblvm2app.so
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(lvm2app_library lvm2app)
    find_path(lvm2app_include lvm2app.h
        PATHS ${CMAKE_BINARY_DIR}/include
    )

    if (lvm2app_library AND lvm2app_include)
        add_library(lvm2app IMPORTED SHARED)
        set_target_properties(lvm2app PROPERTIES
            IMPORTED_LOCATION ${lvm2app_library}
        )
        set(LVM2APP_LIBRARIES lvm2app)
        set(LVM2APP_FOUND TRUE)
    else()
        message(WARNING "lvm2app library or its headers were not found!")
    endif()
endif()
