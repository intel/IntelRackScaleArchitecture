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

function(install_configuration)
    include(ConfigurationPackage OPTIONAL)

    set(CONFIGURATION_SOURCE_DIR
        ${CMAKE_CURRENT_LIST_DIR}/../common/configuration
        CACHE PATH "Configuration source path"
    )

    # First find library to decide if install it
    find_package(Configuration)

    set(source_dir ${CONFIGURATION_SOURCE_DIR})
    set(binary_dir ${CMAKE_BINARY_DIR}/configuration/build)

    if ((NOT ${CONFIGURATION_FOUND} AND EXISTS ${source_dir})
         OR (${CONFIGURATION_FOUND} AND EXISTS ${binary_dir}))
        file(MAKE_DIRECTORY ${binary_dir})

        set(ARGS)
        list(APPEND ARGS -DCMAKE_PREFIX_PATH:PATH=${CMAKE_BINARY_DIR})
        list(APPEND ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR})

        if (CMAKE_CROSSCOMPILING)
            if (CMAKE_TOOLCHAIN_FILE AND EXISTS ${CMAKE_TOOLCHAIN_FILE})
                list(APPEND ARGS -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE})
            endif()

            list(APPEND ARGS -DCMAKE_FIND_ROOT_PATH:PATH=${CMAKE_BINARY_DIR})
        endif()

        if (CMAKE_BUILD_TYPE)
            list(APPEND ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE})
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} ${ARGS} ${source_dir}
            WORKING_DIRECTORY ${binary_dir}
            RESULT_VARIABLE result
        )

        if (NOT ${result} EQUAL 0)
            message(FATAL_ERROR "Error occurs when configure project")
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${binary_dir} --target all
                -- ${BUILD_EXTRA_ARGS}
            WORKING_DIRECTORY ${binary_dir}
            RESULT_VARIABLE result
        )

        if (NOT ${result} EQUAL 0)
            message(FATAL_ERROR "Error occurs when build project")
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${binary_dir} --target install
            WORKING_DIRECTORY ${binary_dir}
            RESULT_VARIABLE result
        )

        if (NOT ${result} EQUAL 0)
            message(FATAL_ERROR "Error occurs when install project")
        endif()
    else()
        message(FATAL_ERROR "${source_dir} not found")
    endif()
endfunction()

install_configuration()
find_package(Configuration)
