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

import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.CollectionDtoType;
import com.intel.rsa.podm.business.dto.ConsoleDto;
import com.intel.rsa.podm.business.dto.ManagerDto;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.ManagerCollection;
import com.intel.rsa.podm.business.entities.assets.ManagerCollectionProvider;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.properties.Console;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Collection;
import java.util.List;

import static com.intel.rsa.podm.business.services.Contexts.asBladeContexts;
import static com.intel.rsa.podm.business.services.Contexts.asComputeModuleContexts;
import static com.intel.rsa.podm.business.services.Contexts.asDrawerContexts;
import static com.intel.rsa.podm.business.services.Contexts.asFabricModuleContexts;
import static com.intel.rsa.podm.business.services.Contexts.asPodContexts;
import static com.intel.rsa.podm.business.services.Contexts.asSwitchContexts;


@Transactional(value = Transactional.TxType.REQUIRED)
public class ManagerServiceImpl implements ManagerService {
    @Inject
    ManagerCollectionProvider managerCollectionProvider;

    @Inject
    DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getManagerCollection() {
        ManagerCollection managerCollection = managerCollectionProvider.getInstance();

        return new CollectionDto(
                CollectionDtoType.Manager,
                managerCollection.getModified(),
                Contexts.getAsIdList(managerCollection.getMembers()));
    }

    @Override
    public ManagerDto getManager(Context context) throws EntityNotFoundException {
        Manager manager = (Manager) traverser.traverse(context);
        if (manager.isStorageManager()) { // TODO workaround
            throw new EntityNotFoundException("");
        }
        return map(manager);
    }

    @Override
    public boolean isStorageManager(Context context) throws EntityNotFoundException {
        Manager manager = (Manager) traverser.traverse(context);
        return manager.isStorageManager();
    }

    private ManagerDto map(Manager manager) {
        return ManagerDto.newBuilder()
                .id(manager.getId())
                .modified(manager.getModified())
                .model(manager.getModel())
                .name(manager.getName())
                .graphicalConsole(translateConsole(manager.getGraphicalConsole()))
                .serialConsole(translateConsole(manager.getSerialConsole()))
                .commandShell(translateConsole(manager.getCommandShell()))
                .state(manager.getState())
                .health(manager.getHealth())
                .healthRollup(manager.getHealthRollup())
                .currentFirmwareVersion(manager.getFirmwareVersion())
                .managerForBlades(asBladeContexts(manager.getManagedBlades()))
                .managerForDrawers(getManagedChassis(manager))
                .managerForComputeModules(asComputeModuleContexts(manager.getManagedComputeModules()))
                .managerForFabricModules(asFabricModuleContexts(manager.getManagedFabricModules()))
                .managerForSwitches(asSwitchContexts(manager.getManagedSwitches()))
                .managerType(manager.getType())
                .build();
    }

    private List<Context> getManagedChassis(Manager manager) {
        Collection<Pod> managedPods = manager.getManagedPods();
        return managedPods.isEmpty() ? asDrawerContexts(manager.getManagedDrawers()) : asPodContexts(managedPods);
    }

    private ConsoleDto translateConsole(Console console) {
        return ConsoleDto
                .newBuilder()
                .enabled(console.isEnabled())
                .maxConcurrentSessions(console.getMaxConcurrentSessions())
                .connectTypesSupported(console.getConnectTypesSupported())
                .build();
    }
}
