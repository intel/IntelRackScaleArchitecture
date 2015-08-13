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

if (NOT OPENIPMI_FOUND)
    find_file(OpenIPMI_library libOpenIPMI.so
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(OpenIPMI_library        OpenIPMI)

    find_file(OpenIPMIui_library libOpenIPMIui.so
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(OpenIPMIui_library      OpenIPMIui)

    find_file(OpenIPMIutils_library libOpenIPMIutils.so
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(OpenIPMIutils_library   OpenIPMIutils)

    find_file(OpenIPMIposix_library libOpenIPMIutils.so
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(OpenIPMIposix_library   OpenIPMIposix)

    find_file(OpenIPMIcmdlang_library libOpenIPMIcmdlang.so
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(OpenIPMIcmdlang_library OpenIPMIcmdlang)

    find_file(OpenIPMIpthread_library libOpenIPMIpthread.so
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(OpenIPMIpthread_library OpenIPMIpthread)

    find_path(OpenIPMI_include "OpenIPMI/ipmi_mc.h"
        PATHS ${CMAKE_BINARY_DIR}/include
    )

    if (OpenIPMI_library AND OpenIPMIui_library AND OpenIPMIutils_library
    AND OpenIPMIposix_library AND OpenIPMIcmdlang_library
    AND OpenIPMIpthread_library AND OpenIPMI_include)
        add_library(OpenIPMI IMPORTED SHARED)
        set_target_properties(OpenIPMI PROPERTIES
            IMPORTED_LOCATION ${OpenIPMI_library}
        )

        add_library(OpenIPMIui IMPORTED SHARED)
        set_target_properties(OpenIPMIui PROPERTIES
            IMPORTED_LOCATION ${OpenIPMIui_library}
        )

        add_library(OpenIPMIutils IMPORTED SHARED)
        set_target_properties(OpenIPMIutils PROPERTIES
            IMPORTED_LOCATION ${OpenIPMIutils_library}
        )

        add_library(OpenIPMIposix IMPORTED SHARED)
        set_target_properties(OpenIPMIposix PROPERTIES
            IMPORTED_LOCATION ${OpenIPMIposix_library}
        )

        add_library(OpenIPMIcmdlang IMPORTED SHARED)
        set_target_properties(OpenIPMIcmdlang PROPERTIES
            IMPORTED_LOCATION ${OpenIPMIcmdlang_library}
        )

        add_library(OpenIPMIpthread IMPORTED SHARED)
        set_target_properties(OpenIPMIpthread PROPERTIES
            IMPORTED_LOCATION ${OpenIPMIpthread_library}
        )

        get_filename_component(OPENIPMI_LIBRARY_DIRS ${OpenIPMI_library}
            DIRECTORY
        )
        set(OPENIPMI_LIBRARIES
            OpenIPMI
            OpenIPMIui
            OpenIPMIutils
            OpenIPMIposix
            OpenIPMIcmdlang
            OpenIPMIpthread
        )
        set(OPENIPMI_INCLUDE_DIRS ${OpenIPMI_include})
        set(OPENIPMI_FOUND TRUE)
    endif()
endif()
