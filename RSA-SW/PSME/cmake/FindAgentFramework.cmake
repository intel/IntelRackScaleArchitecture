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

if (NOT AGENT_FRAMEWORK_FOUND)
    find_file(agent-framework_library libagent-framework.a
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(agent-framework_library agent-framework)

    find_file(agent-commands_library libagent-commands.a
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(agent-commands_library agent-commands)

    find_path(agent-framework_include "agent-framework/version.hpp"
        PATHS ${CMAKE_BINARY_DIR}/include
    )

    if (agent-framework_library AND agent-commands_library
    AND agent-framework_include)
        add_library(agent-framework IMPORTED STATIC)
        set_target_properties(agent-framework PROPERTIES
            IMPORTED_LOCATION ${agent-framework_library}
        )

        get_filename_component(agent-framework_library_dir
            ${agent-framework_library} DIRECTORY
        )

        add_library(agent-commands IMPORTED STATIC)
        set_target_properties(agent-commands PROPERTIES
            IMPORTED_LOCATION ${agent-commands_library}
        )

        set(AGENT_FRAMEWORK_INCLUDE_DIRS
            ${agent-framework_include}
        )
        set(AGENT_FRAMEWORK_LIBRARY_DIRS ${agent-framework_library_dir})
        set(AGENT_FRAMEWORK_LIBRARIES
            -Wl,--whole-archive agent-commands -Wl,--no-whole-archive
            agent-framework
        )
        set(AGENT_FRAMEWORK_FOUND TRUE)
    endif()
endif()
