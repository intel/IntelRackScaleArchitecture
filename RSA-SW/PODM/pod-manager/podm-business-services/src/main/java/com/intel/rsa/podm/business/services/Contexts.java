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

package com.intel.rsa.podm.business.services;

import com.intel.rsa.common.types.Id;
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
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.services.context.Context;

import java.util.Collection;
import java.util.List;
import java.util.function.Function;

import static com.intel.rsa.podm.business.services.context.Context.contextOf;
import static com.intel.rsa.podm.business.services.context.ContextType.BLADE;
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
import static java.util.stream.Collectors.toList;
import static java.util.stream.StreamSupport.stream;

public final class Contexts {
    private Contexts() {
    }

    public static Collection<Id> getAsIdList(Collection<? extends DomainObject> domainObjects) {
        return transform(domainObjects, DomainObject::getId);
    }

    public static List<Context> asLogicalDriveContexts(Collection<LogicalDrive> logicalDrives) {
        return transform(logicalDrives, Contexts::toContext);
    }

    public static List<Context> asPhysicalDriveContexts(Collection<PhysicalDrive> physicalDrives) {
        return transform(physicalDrives, Contexts::toContext);
    }

    public static List<Context> asStorageServicesContexts(Collection<StorageService> storageServices) {
        return transform(storageServices, Contexts::toContext);
    }

    public static List<Context> asRemoteTargetContexts(Collection<RemoteTarget> remoteTargets) {
        return transform(remoteTargets, Contexts::toContext);
    }

    public static List<Context> asPodContexts(Collection<Pod> pods) {
        return transform(pods, Contexts::toContext);
    }

    public static List<Context> asDrawerContexts(Collection<Drawer> drawers) {
        return transform(drawers, Contexts::toContext);
    }

    public static List<Context> asComputeModuleContexts(Collection<ComputeModule> computeModules) {
        return transform(computeModules, Contexts::toContext);
    }

    public static List<Context> asManagerContexts(Collection<Manager> managers) {
        return transform(managers, Contexts::toContext);
    }

    public static List<Context> asFabricModuleContexts(Collection<FabricModule> fabricModules) {
        return transform(fabricModules, Contexts::toContext);
    }

    public static List<Context> asBladeContexts(Collection<Blade> blades) {
        return transform(blades, Contexts::toContext);
    }

    public static List<Context> asSwitchContexts(Collection<Switch> switches) {
        return transform(switches, Contexts::toContext);
    }

    public static Context toContext(Manager manager) {
        return contextOf(manager.getId(), MANAGER);
    }

    public static Context toContext(Pod pod) {
        return Context.contextOf(pod.getId(), POD);
    }

    public static Context toContext(Rack rack) {
        return toContext(rack.getPod()).child(rack.getId(), RACK);
    }

    public static Context toContext(Drawer drawer) {
        return toContext(drawer.getRack()).child(drawer.getId(), DRAWER);
    }

    public static Context toContext(ComputeModule computeModule) {
        return toContext(computeModule.getDrawer()).child(computeModule.getId(), COMPUTE_MODULE);
    }

    public static Context toContext(Blade blade) {
        return toContext(blade.getComputeModule()).child(blade.getId(), BLADE);
    }

    public static Context toContext(Processor processor) {
        return toContext(processor.getBlade()).child(processor.getId(), PROCESSOR);
    }

    public static Context toContext(StorageController storageController) {
        return toContext(storageController.getBlade()).child(storageController.getId(), STORAGE_CONTROLLER);
    }

    public static Context toContext(Drive drive) {
        return toContext(drive.getStorageController()).child(drive.getId(), DRIVE);
    }

    public static Context toContext(NetworkInterface networkInterface) {
        Context parentContext = networkInterface.getBlade() != null
                ? toContext(networkInterface.getBlade())
                : toContext(networkInterface.getManager());

        return parentContext.child(networkInterface.getId(), NETWORK_INTERFACE);
    }

    public static Context toContext(Memory memory) {
        return toContext(memory.getBlade()).child(memory.getId(), MEMORY);
    }

    public static Context toContext(FabricModule fabricModule) {
        return toContext(fabricModule.getDrawer()).child(fabricModule.getId(), FABRIC_MODULE);
    }

    public static Context toContext(Switch aSwitch) {
        return toContext(aSwitch.getFabricModule()).child(aSwitch.getId(), SWITCH);
    }

    public static Context toContext(StorageService storageService) {
        if (storageService == null) {
            return null;
        }

        return contextOf(storageService.getId(), STORAGE_SERVICE);
    }

    public static Context toContext(PhysicalDrive physicalDrive) {
        Id storageServiceId = physicalDrive.getStorageService().getId();
        return contextOf(storageServiceId, STORAGE_SERVICE).child(physicalDrive.getId(), PHYSICAL_DRIVE);
    }

    public static Context toContext(LogicalDrive logicalDrive) {
        if (logicalDrive == null) {
            return null;
        }

        return toContext(logicalDrive.getStorageService()).child(logicalDrive.getId(), LOGICAL_DRIVE);
    }

    public static Context toContext(RemoteTarget remoteTarget) {
        if (remoteTarget == null) {
            return null;
        }

        return toContext(remoteTarget.getStorageService()).child(remoteTarget.getId(), REMOTE_TARGET);
    }

    private static <T, R> List<R> transform(Iterable<T> items, Function<T, R> function) {
        return stream(items.spliterator(), false).map(function).collect(toList());
    }
}
