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

package com.intel.rsa.podm.business.services.context;

import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.ComputeModule;
import com.intel.rsa.podm.business.entities.assets.Drawer;
import com.intel.rsa.podm.business.entities.assets.Drive;
import com.intel.rsa.podm.business.entities.assets.FabricModule;
import com.intel.rsa.podm.business.entities.assets.LogicalDrive;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.Memory;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.entities.assets.PhysicalDrive;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.Processor;
import com.intel.rsa.podm.business.entities.assets.Rack;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.assets.StorageController;
import com.intel.rsa.podm.business.entities.assets.StorageService;
import com.intel.rsa.podm.business.entities.assets.Switch;
import com.intel.rsa.podm.business.entities.assets.SwitchPort;
import com.intel.rsa.podm.business.entities.assets.VlanNetworkInterface;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.components.ComposedNode;

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

public class ContextTypeToDomainObjectMapper {
    static final Map<ContextType, Class<? extends DomainObject>> MAPPING;

    static {
        HashMap<ContextType, Class<? extends DomainObject>> map = new HashMap<>();

        map.put(POD, Pod.class);
        map.put(RACK, Rack.class);
        map.put(DRAWER, Drawer.class);
        map.put(COMPUTE_MODULE, ComputeModule.class);
        map.put(BLADE, Blade.class);
        map.put(PROCESSOR, Processor.class);
        map.put(MEMORY, Memory.class);
        map.put(STORAGE_CONTROLLER, StorageController.class);
        map.put(DRIVE, Drive.class);
        map.put(FABRIC_MODULE, FabricModule.class);
        map.put(SWITCH, Switch.class);
        map.put(SWITCH_PORT, SwitchPort.class);
        map.put(VLAN, VlanNetworkInterface.class);
        map.put(MANAGER, Manager.class);
        map.put(NETWORK_INTERFACE, NetworkInterface.class);
        map.put(COMPOSED_NODE, ComposedNode.class);
        map.put(STORAGE_SERVICE, StorageService.class);
        map.put(PHYSICAL_DRIVE, PhysicalDrive.class);
        map.put(LOGICAL_DRIVE, LogicalDrive.class);
        map.put(REMOTE_TARGET, RemoteTarget.class);

        MAPPING = unmodifiableMap(map);
    }

    public Class<? extends DomainObject> get(ContextType contextType) {
        if (contextType == null) {
            throw new IllegalArgumentException("contextType must not be null");
        }

        return MAPPING.get(contextType);
    }
}
