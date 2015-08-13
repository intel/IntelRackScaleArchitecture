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

import java.util.Collection;
import java.util.function.Function;

import static java.lang.String.format;
import static java.util.stream.Collectors.toList;

enum ChildrenExtractor {
    RACK_FROM_POD(Pod.class, Rack.class, Pod::getRacks),
    DRAWER_FROM_RACK(Rack.class, Drawer.class, Rack::getDrawers),
    COMPUTE_MODULE_FROM_DRAWER(Drawer.class, ComputeModule.class, Drawer::getComputeModules),
    BLADE_FROM_COMPUTE_MODULE(ComputeModule.class, Blade.class, ComputeModule::getBlades),
    PROCESSOR_FROM_BLADE(Blade.class, Processor.class, Blade::getProcessors),
    MEMORY_FROM_BLADE(Blade.class, Memory.class, Blade::getMemory),
    NETWORK_INTERFACE_FROM_BLADE(Blade.class, NetworkInterface.class, Blade::getEthernetInterfaces),
    STORAGE_CONTROLLER_FROM_BLADE(Blade.class, StorageController.class, Blade::getStorageControllers),
    DRIVE_FROM_STORAGE_CONTROLLER(StorageController.class, Drive.class, StorageController::getDrives),
    FABRIC_MODULE_FROM_DRAWER(Drawer.class, FabricModule.class, Drawer::getFabricModules),
    SWITCH_FROM_FABRIC_MODULE(FabricModule.class, Switch.class, FabricModule::getSwitches),
    PORT_FROM_SWITCH(Switch.class, SwitchPort.class, Switch::getSwitchPorts),
    VLAN_FROM_PORT(SwitchPort.class, VlanNetworkInterface.class, SwitchPort::getVlans),
    NETWORK_INTERFACE_FROM_MANAGER(Manager.class, NetworkInterface.class, Manager::getNetworkInterfaces),
    PHYSICAL_DRIVE_FROM_STORAGE_SERVICE(StorageService.class, PhysicalDrive.class, StorageService::getPhysicalDrives),
    LOGICAL_DRIVE_FROM_STORAGE_SERVICE(StorageService.class, LogicalDrive.class, StorageService::getLogicalDrives),
    REMOTE_TARGET_FROM_STORAGE_SERVICE(StorageService.class, RemoteTarget.class, StorageService::getRemoteTargets);

    private final Class<? extends DomainObject> parentClass;
    private final Class<? extends DomainObject> childClass;
    private final Function<DomainObject, Collection<DomainObject>> extractor;

    <P extends DomainObject, C extends DomainObject>
    ChildrenExtractor(Class<P> parentClass, Class<C> childClass, Function<P, Collection<C>> extractor) {
        this.parentClass = parentClass;
        this.childClass = childClass;
        this.extractor = p -> extractor.apply((P) p).stream().collect(toList());
    }

    public Class<? extends DomainObject> getParentClass() {
        return parentClass;
    }

    public Class<? extends DomainObject> getChildClass() {
        return childClass;
    }

    public Collection<DomainObject> extractFrom(DomainObject parent) {
        if (!getParentClass().isInstance(parent)) {
            throw new IllegalArgumentException(format("parent must be instnace of %s", getParentClass()));
        }

        return extractor.apply(parent);
    }

    public boolean isApplicableFor(DomainObject parent, DomainObject child) {
        return parentClass.isInstance(parent) && childClass.isInstance(child);
    }
}
