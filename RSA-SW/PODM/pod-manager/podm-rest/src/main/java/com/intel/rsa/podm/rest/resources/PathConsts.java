/*
 * Copyright (c) 2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.intel.rsa.podm.rest.resources;

import java.util.ArrayList;
import java.util.Collection;

import static com.google.common.collect.Lists.newArrayList;

public final class PathConsts {
    static final String POD_ID_PARAM = "podId";
    static final String RACK_ID_PARAM = "rackId";
    static final String DRAWER_ID_PARAM = "drawerId";
    static final String COMPUTE_MODULE_ID_PARAM = "computeModuleId";
    static final String COMPOSED_NODE_ID_PARAM = "nodeId";
    static final String BLADE_ID_PARAM = "bladeId";
    static final String PROCESSOR_ID_PARAM = "processorId";
    static final String MEMORY_ID_PARAM = "memoryId";
    static final String STORAGE_CONTROLLER_ID_PARAM = "storageControllerId";
    static final String DRIVE_ID_PARAM = "driveId";
    static final String PHYSICAL_DRIVE_ID_PARAM = "physicalDriveId";
    static final String LOGICAL_DRIVE_ID_PARAM = "logicalDriveId";
    static final String REMOTE_TARGET_ID_PARAM = "remoteTargetId";
    static final String FABRIC_MODULE_ID_PARAM = "fabricModuleId";
    static final String SWITCH_ID_PARAM = "switchId";
    static final String SWITCH_PORT_ID_PARAM = "portId";
    static final String VLAN_ID_PARAM = "vlanId";
    static final String MANAGER_ID_PARAM = "managerId";
    static final String NETWORK_INTERFACE_ID = "networkInterfaceId";
    static final String STORAGE_SERVICE_ID = "storageServiceId";

    private static final ArrayList<String> TOPOLOGICALLY_ORDERED_PARAMETERS;

    static {
        TOPOLOGICALLY_ORDERED_PARAMETERS = newArrayList(
                POD_ID_PARAM,
                RACK_ID_PARAM,
                DRAWER_ID_PARAM,
                COMPUTE_MODULE_ID_PARAM,
                BLADE_ID_PARAM,
                PROCESSOR_ID_PARAM,
                MEMORY_ID_PARAM,
                STORAGE_CONTROLLER_ID_PARAM,
                DRIVE_ID_PARAM,
                FABRIC_MODULE_ID_PARAM,
                SWITCH_ID_PARAM,
                SWITCH_PORT_ID_PARAM,
                VLAN_ID_PARAM,
                MANAGER_ID_PARAM,
                NETWORK_INTERFACE_ID,
                COMPOSED_NODE_ID_PARAM,
                STORAGE_SERVICE_ID,
                PHYSICAL_DRIVE_ID_PARAM,
                LOGICAL_DRIVE_ID_PARAM,
                REMOTE_TARGET_ID_PARAM
        );
    }

    private PathConsts() {
    }

    public static Collection<String> getPathParamNames() {
        return TOPOLOGICALLY_ORDERED_PARAMETERS;
    }
}
