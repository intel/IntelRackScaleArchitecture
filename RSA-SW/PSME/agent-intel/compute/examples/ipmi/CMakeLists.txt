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

set (SOURCES
    main.cpp
)

add_library(ipmi-module-intel
    $<TARGET_OBJECTS:ipmi-intel>
)

add_executable(ipmi-example-intel ${SOURCES})

target_link_libraries(ipmi-example-intel
    ipmi-module-intel
    OpenIPMI
    OpenIPMIposix
    OpenIPMIpthread
    pthread
    ${SAFESTRING_LIBRARIES}
)
