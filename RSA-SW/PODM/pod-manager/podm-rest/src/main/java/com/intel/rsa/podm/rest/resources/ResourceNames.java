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

import java.lang.reflect.Field;
import java.util.HashSet;
import java.util.Set;

import static java.util.Collections.unmodifiableSet;

public final class ResourceNames {
    public static final Set<String> ALL_RESOURCE_NAMES;

    public static final String PODS_RESOURCE_NAME = "Pods";
    public static final String RACKS_RESOURCE_NAME = "Racks";
    public static final String DRAWERS_RESOURCE_NAME = "Drawers";
    public static final String COMPOSED_NODES_RESOURCE_NAME = "Systems";
    public static final String COMPUTE_MODULES_RESOURCE_NAME = "ComputeModules";
    public static final String BLADES_RESOURCE_NAME = "Blades";
    public static final String PROCESSORS_RESOURCE_NAME = "Processors";
    public static final String STORAGE_CONTROLLERS_RESOURCE_NAME = "StorageControllers";
    public static final String DRIVES_RESOURCE_NAME = "Drives";
    public static final String FABRIC_MODULES_RESOURCE_NAME = "FabricModules";
    public static final String SWITCHES_RESOURCE_NAME = "Switches";
    public static final String SWITCH_PORTS_RESOURCE_NAME = "Ports";
    public static final String MANAGERS_RESOURCE_NAME = "Managers";
    public static final String NETWORK_SERVICE_RESOURCE_NAME  = "NetworkService";
    public static final String NETWORK_INTERFACES_RESOURCE_NAME = "EthernetInterfaces";
    public static final String VLANS_RESOURCE_NAME = "VLANs";
    public static final String MEMORY_RESOURCE_NAME = "Memory";
    public static final String STORAGE_SERVICES_RESOURCE_NAME = "Services";
    public static final String LOGICAL_DRIVES_RESOURCE_NAME = "LogicalDrives";
    public static final String PHYSICAL_DRIVES_RESOURCE_NAME = "Drives";
    public static final String REMOTE_TARGETS_RESOURCE_NAME = "Targets";

    /**
     * It fills ALL_RESOURCE_NAMES collection with all static String field values
     */
    static {
        Set<String> names = new HashSet<>();

        for (Field field : ResourceNames.class.getDeclaredFields()) {
            if (field.getType() != String.class) {
                continue;
            }

            try {
                names.add((String) field.get(null));
            } catch (IllegalAccessException e) {
                throw new RuntimeException(e);
            }
        }

        ALL_RESOURCE_NAMES = unmodifiableSet(names);
    }

    private ResourceNames() {
    }
}
