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

package com.intel.rsa.podm.discovery.external.linker;

import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.ComputeModule;
import com.intel.rsa.podm.business.entities.assets.Drawer;
import com.intel.rsa.podm.business.entities.assets.Drive;
import com.intel.rsa.podm.business.entities.assets.FabricModule;
import com.intel.rsa.podm.business.entities.assets.LogicalDrive;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.ManagerCollection;
import com.intel.rsa.podm.business.entities.assets.Memory;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.entities.assets.NetworkService;
import com.intel.rsa.podm.business.entities.assets.PhysicalDrive;
import com.intel.rsa.podm.business.entities.assets.Processor;
import com.intel.rsa.podm.business.entities.assets.Rack;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.assets.StorageController;
import com.intel.rsa.podm.business.entities.assets.StorageService;
import com.intel.rsa.podm.business.entities.assets.StorageServiceCollection;
import com.intel.rsa.podm.business.entities.assets.Switch;
import com.intel.rsa.podm.business.entities.assets.SwitchPort;
import com.intel.rsa.podm.business.entities.assets.VlanNetworkInterface;
import com.intel.rsa.podm.business.entities.base.DomainObject;

import javax.enterprise.context.Dependent;
import javax.enterprise.inject.Produces;
import java.util.ArrayList;
import java.util.Collection;

import static java.util.Collections.unmodifiableCollection;

@Dependent
public class LinkerCollectionProducer {
    private static final Collection<Linker> LINKERS = new ArrayList<>();

    static {
        register(StorageServiceCollection.class, StorageService.class, "contains", StorageServiceCollection::addStorageService);
        register(ManagerCollection.class, Manager.class, "contains", ManagerCollection::link);
        register(StorageService.class, PhysicalDrive.class, "drives", StorageService::addPhysicalDrive);
        register(StorageService.class, LogicalDrive.class, "logicalDrives", StorageService::addLogicalDrive);
        register(StorageService.class, RemoteTarget.class, "targets", StorageService::addRemoteTarget);
        register(StorageService.class, Manager.class, "managedBy", StorageService::linkManager);
        register(LogicalDrive.class, PhysicalDrive.class, "physicalDrives", LogicalDrive::addPhysicalDrive);
        register(LogicalDrive.class, LogicalDrive.class, "masterDrive", LogicalDrive::setMasterDrive);
        register(LogicalDrive.class, LogicalDrive.class, "logicalDrives", LogicalDrive::use);
        register(LogicalDrive.class, Manager.class, "managedBy", LogicalDrive::linkManager);
        register(RemoteTarget.class, Manager.class, "managedBy", RemoteTarget::linkManager);
        register(RemoteTarget.class, LogicalDrive.class, "logicalDrives", RemoteTarget::addLogicalDrive);
        register(Manager.class, NetworkService.class, "networkService", Manager::setNetworkService);
        register(Manager.class, NetworkInterface.class, "simpleNetwork", Manager::contain);
        register(Rack.class, Drawer.class, "contains", Rack::contain);
        register(Drawer.class, ComputeModule.class, "computeModules", Drawer::contain);
        register(Drawer.class, FabricModule.class, "fabricModules", Drawer::contain);
        register(Drawer.class, Manager.class, "managedBy", Drawer::linkManager);
        register(ComputeModule.class, Blade.class, "blades", ComputeModule::contain);
        register(ComputeModule.class, Manager.class, "managedBy", ComputeModule::linkManager);
        register(FabricModule.class, Switch.class, "switches", FabricModule::contain);
        register(FabricModule.class, Manager.class, "managedBy", FabricModule::linkManager);
        register(SwitchPort.class, VlanNetworkInterface.class, "vlans", SwitchPort::contain);
        register(Blade.class, Processor.class, "processors", Blade::contain);
        register(Blade.class, Memory.class, "memory", Blade::contain);
        register(Blade.class, StorageController.class, "storageControllers", Blade::contain);
        register(Blade.class, NetworkInterface.class, "simpleNetwork", Blade::contain);
        register(Blade.class, Manager.class, "managedBy", Blade::linkManager);
        register(StorageController.class, Drive.class, "drives", StorageController::contain);
        register(Switch.class, SwitchPort.class, "ports", Switch::contain);
        register(Switch.class, Manager.class, "managedBy", Switch::linkManager);
    }


    @Produces
    public Collection<Linker> create() {
        return unmodifiableCollection(LINKERS);
    }

    private static <S extends DomainObject, T extends DomainObject>
    void register(Class<S> sourceClass, Class<T> targetClass, String linkName, Linker.LinkMethod<S, T> method) {
        LINKERS.add(new Linker(sourceClass, targetClass, linkName, method));
    }
}
