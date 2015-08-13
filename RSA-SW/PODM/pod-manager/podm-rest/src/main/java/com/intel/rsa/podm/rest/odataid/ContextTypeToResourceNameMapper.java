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

package com.intel.rsa.podm.rest.odataid;

import com.intel.rsa.podm.business.services.context.ContextType;

import java.util.HashMap;
import java.util.Map;

import static com.intel.rsa.podm.business.services.context.ContextType.BLADE;
import static com.intel.rsa.podm.business.services.context.ContextType.COMPOSED_NODE;
import static com.intel.rsa.podm.business.services.context.ContextType.COMPUTE_MODULE;
import static com.intel.rsa.podm.business.services.context.ContextType.DRAWER;
import static com.intel.rsa.podm.business.services.context.ContextType.DRIVE;
import static com.intel.rsa.podm.business.services.context.ContextType.FABRIC_MODULE;
import static com.intel.rsa.podm.business.services.context.ContextType.LOGICAL_DRIVE;
import static com.intel.rsa.podm.business.services.context.ContextType.MANAGER;
import static com.intel.rsa.podm.business.services.context.ContextType.MEMORY;
import static com.intel.rsa.podm.business.services.context.ContextType.NETWORK_INTERFACE;
import static com.intel.rsa.podm.business.services.context.ContextType.PHYSICAL_DRIVE;
import static com.intel.rsa.podm.business.services.context.ContextType.POD;
import static com.intel.rsa.podm.business.services.context.ContextType.PROCESSOR;
import static com.intel.rsa.podm.business.services.context.ContextType.RACK;
import static com.intel.rsa.podm.business.services.context.ContextType.REMOTE_TARGET;
import static com.intel.rsa.podm.business.services.context.ContextType.STORAGE_CONTROLLER;
import static com.intel.rsa.podm.business.services.context.ContextType.STORAGE_SERVICE;
import static com.intel.rsa.podm.business.services.context.ContextType.SWITCH;
import static com.intel.rsa.podm.business.services.context.ContextType.SWITCH_PORT;
import static com.intel.rsa.podm.business.services.context.ContextType.VLAN;
import static java.util.Collections.unmodifiableMap;

public class ContextTypeToResourceNameMapper {
    static final Map<ContextType, String> MAPPING;

    static {
        HashMap<ContextType, String> map = new HashMap<>();
        map.put(POD, "Pods");
        map.put(RACK, "Racks");
        map.put(DRAWER, "Drawers");
        map.put(COMPUTE_MODULE, "ComputeModules");
        map.put(COMPOSED_NODE, "Systems");
        map.put(BLADE, "Blades");
        map.put(PROCESSOR, "Processors");
        map.put(MEMORY, "Memory");
        map.put(STORAGE_CONTROLLER, "StorageControllers");
        map.put(DRIVE, "Drives");
        map.put(FABRIC_MODULE, "FabricModules");
        map.put(SWITCH, "Switches");
        map.put(SWITCH_PORT, "Ports");
        map.put(VLAN, "VLANs");
        map.put(MANAGER, "Managers");
        map.put(NETWORK_INTERFACE, "EthernetInterfaces");
        map.put(STORAGE_SERVICE, "Services");
        map.put(PHYSICAL_DRIVE, "Drives");
        map.put(LOGICAL_DRIVE, "LogicalDrives");
        map.put(REMOTE_TARGET, "Targets");

        MAPPING = unmodifiableMap(map);
    }

    public String get(ContextType type) {
        if (type == null) {
            throw new IllegalArgumentException("contextType must not be null");
        }

        return MAPPING.get(type);
    }
}
