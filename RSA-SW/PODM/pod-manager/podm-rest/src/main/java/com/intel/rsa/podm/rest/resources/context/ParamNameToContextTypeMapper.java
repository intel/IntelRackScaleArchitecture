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

package com.intel.rsa.podm.rest.resources.context;

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
import static java.lang.String.format;
import static java.util.Collections.unmodifiableMap;

final class ParamNameToContextTypeMapper {
    static final Map<String, ContextType> MAPPING;

    static {
        Map<String, ContextType> map = new HashMap<>();

        map.put("podId", POD);
        map.put("rackId", RACK);
        map.put("drawerId", DRAWER);
        map.put("computeModuleId", COMPUTE_MODULE);
        map.put("nodeId", COMPOSED_NODE);
        map.put("bladeId", BLADE);
        map.put("processorId", PROCESSOR);
        map.put("memoryId", MEMORY);
        map.put("storageControllerId", STORAGE_CONTROLLER);
        map.put("driveId", DRIVE);
        map.put("fabricModuleId", FABRIC_MODULE);
        map.put("switchId", SWITCH);
        map.put("portId", SWITCH_PORT);
        map.put("vlanId", VLAN);
        map.put("managerId", MANAGER);
        map.put("networkInterfaceId", NETWORK_INTERFACE);
        map.put("storageServiceId", STORAGE_SERVICE);
        map.put("physicalDriveId", PHYSICAL_DRIVE);
        map.put("logicalDriveId", LOGICAL_DRIVE);
        map.put("remoteTargetId", REMOTE_TARGET);

        MAPPING = unmodifiableMap(map);
    }

    public boolean contains(String paramName) {
        return MAPPING.containsKey(paramName);
    }

    public ContextType get(String paramName) {
        if (!MAPPING.containsKey(paramName)) {
            String msg = format("paramName '%s' is not known", paramName);
            throw new IllegalArgumentException(msg);
        }

        return MAPPING.get(paramName);
    }
}
