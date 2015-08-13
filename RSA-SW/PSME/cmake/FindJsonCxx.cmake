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

if (NOT JSONCXX_FOUND)
    find_file(json-cxx_library libjson-cxx.a
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(json-cxx_library json-cxx)
    find_path(json-cxx_include "json/json.hpp"
        PATHS ${CMAKE_BINARY_DIR}/include
    )

    if (json-cxx_library AND json-cxx_include)
        add_library(json-cxx IMPORTED STATIC)
        set_target_properties(json-cxx PROPERTIES
            IMPORTED_LOCATION ${json-cxx_library}
        )

        get_filename_component(JSONCXX_LIBRARY_DIRS ${json-cxx_library}
            DIRECTORY
        )

        set(JSONCXX_LIBRARIES json-cxx)
        set(JSONCXX_INCLUDE_DIRS ${json-cxx_include})
        set(JSONCXX_FOUND TRUE)
    endif()
endif()
