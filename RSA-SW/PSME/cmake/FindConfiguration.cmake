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

# - Try to find Configuration
# Once done this will define
#  CONFIGURATION_FOUND - System has configuration library
#  CONFIGURATION_INCLUDE_DIRS - The configuration include directories
#  CONFIGURATION_LIBRARIES - The libraries needed to use configuration

find_path(CONFIGURATION_INCLUDE_DIR configuration/configuration.hpp
    HINTS ${CMAKE_BINARY_DIR}/include
)

find_file(CONFIGURATION_LIBRARY NAMES libconfiguration.a
    HINTS ${CMAKE_BINARY_DIR}/lib
)

find_file(CONFIGURATION_LIBRARY NAMES libconfiguration.so
    HINTS ${CMAKE_BINARY_DIR}/lib
)

find_library(CONFIGURATION_LIBRARY NAMES configuration
    HINTS ${CMAKE_BINARY_DIR}/lib
)

set(CONFIGURATION_LIBRARIES ${CONFIGURATION_LIBRARY} -lcrypt)
set(CONFIGURATION_INCLUDE_DIRS ${CONFIGURATION_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CONFIGURATION_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(CONFIGURATION DEFAULT_MSG
    CONFIGURATION_LIBRARY
    CONFIGURATION_INCLUDE_DIR
)

mark_as_advanced(CONFIGURATION_INCLUDE_DIR CONFIGURATION_LIBRARY)
