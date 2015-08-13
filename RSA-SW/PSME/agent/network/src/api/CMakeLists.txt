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
    # Interface
    switch_vlan.cpp
    switch_port_info.cpp
    switch_info.cpp
    remote_switch_info.cpp
)

set(SOURCES ${SOURCES}
    # IES Interface
    ies/switch_port_info.cpp
    ies/switch_vlan.cpp
    ies/switch_info.cpp
)

if (NL3_FOUND)
    set_source_files_properties(
        netlink/message.cpp
        netlink/vlan_message.cpp
        COMPILE_FLAGS "-Wno-missing-field-initializers -Wno-old-style-cast -Wno-cast-align")

    set(SOURCES ${SOURCES}
        # Netlink Interface
        netlink/message.cpp
        netlink/vlan_message.cpp
        netlink/socket.cpp
        netlink/ethtool.cpp
        netlink/switch_port_info.cpp
        netlink/switch_vlan.cpp
        netlink/switch_info.cpp
    )

    if (UDEV_FOUND)
        set(SOURCES ${SOURCES}
            # sysfs interface
            netlink/sysfs.cpp
        )
    endif()
endif()

set(SOURCES ${SOURCES}
    # DCRP Interface
    dcrp/remote_switch_info.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set_source_files_properties(
        netlink/message.cpp
        netlink/vlan_message.cpp
        netlink/socket.cpp
        dcrp/remote_switch_info.cpp
        PROPERTIES
        COMPILE_FLAGS "-Wno-old-style-cast -Wno-missing-field-initializers -Wno-sign-conversion"
    )
endif()

add_subdirectory(ies)
add_subdirectory(netlink)
add_subdirectory(dcrp)

add_library(api OBJECT ${SOURCES})
