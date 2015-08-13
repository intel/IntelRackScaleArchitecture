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

package com.intel.rsa.podm.discovery.internal;

import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.ManagerCollection;
import com.intel.rsa.podm.business.entities.assets.ManagerCollectionProvider;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.properties.Console;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.configuration.VersionLoader;

import javax.enterprise.context.ApplicationScoped;
import javax.inject.Inject;
import java.util.Collection;

import static com.intel.rsa.common.types.ManagerType.MANAGEMENT_CONTROLLER;
import static com.intel.rsa.common.utils.IterableHelper.single;

@ApplicationScoped
public class PodManagerDiscoveryHandler {

    @Inject
    DomainObjectRepository repository;

    @Inject
    ManagerCollectionProvider managerCollectionProvider;

    @Inject
    PodManagerNetworkInterfaceDiscoveryHandler podManagerNetworkInterfaceDiscoveryHandler;

    @Inject
    PodManagerNetworkServiceDiscoveryHandler podManagerNetworkServiceDiscoveryHandler;

    @Inject
    VersionLoader versionLoader;

    public Manager getManagerForPod(Pod pod) {
        if (pod == null) {
            throw new IllegalArgumentException("Cannot create Manager for a null Pod");
        }

        ManagerCollection managerCollection = managerCollectionProvider.getInstance();

        Collection<Manager> podManagers = pod.getManagers();
        if (!podManagers.isEmpty()) {
            return single(podManagers);
        } else {
            Manager manager = createManagerForPod();
            managerCollection.link(manager);
            pod.linkManager(manager);
            return manager;
        }
    }

    private Manager createManagerForPod() {
        Manager manager = repository.create(Manager.class);

        manager.setFirmwareVersion(versionLoader.loadAppVersion());

        manager.updateModified();
        manager.setHealth(Health.OK);
        manager.setHealthRollup(Health.OK);
        manager.setName("RSA Pod Manager");
        manager.setGraphicalConsole(createDisabledConsole());
        manager.setSerialConsole(createDisabledConsole());
        manager.setCommandShell(createDisabledConsole());
        manager.setState(State.ENABLED);
        manager.setType(MANAGEMENT_CONTROLLER);

        podManagerNetworkInterfaceDiscoveryHandler.addNetworkInterfaces(manager);
        podManagerNetworkServiceDiscoveryHandler.addNetworkService(manager);

        return manager;
    }

    private Console createDisabledConsole() {
        Console console = repository.create(Console.class);
        console.setEnabled(false);
        return console;
    }
}
