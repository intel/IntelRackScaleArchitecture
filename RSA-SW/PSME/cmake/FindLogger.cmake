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

if (NOT LOGGER_FOUND)
    find_file(logger_library liblogger.a
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(logger_library logger)
    find_path(logger_include "logger/logger.h"
        PATHS ${CMAKE_BINARY_DIR}/include
    )

    if (logger_library AND logger_include)
        add_library(logger IMPORTED STATIC)
        set_target_properties(logger PROPERTIES
            IMPORTED_LOCATION ${logger_library}
        )

        get_filename_component(LOGGER_LIBRARY_DIR ${logger_library} DIRECTORY)

        set(LOGGER_INCLUDE_DIRS ${logger_include})
        set(LOGGER_LIBRARY_DIRS ${LOGGER_LIBRARY_DIR})
        set(LOGGER_LIBRARIES logger pthread)
        set(LOGGER_FOUND TRUE)
    endif()
endif()
