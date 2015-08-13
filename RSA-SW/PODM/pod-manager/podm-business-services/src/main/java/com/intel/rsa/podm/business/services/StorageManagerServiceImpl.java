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

import com.intel.rsa.podm.business.dto.ConsoleDto;
import com.intel.rsa.podm.business.dto.StorageManagerDto;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.properties.Console;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.services.Contexts.asLogicalDriveContexts;
import static com.intel.rsa.podm.business.services.Contexts.asRemoteTargetContexts;
import static com.intel.rsa.podm.business.services.Contexts.asStorageServicesContexts;

@Transactional(value = Transactional.TxType.REQUIRED)
public class StorageManagerServiceImpl implements StorageManagerService {

    @Inject
    DomainObjectTreeTraverser traverser;

    @Override
    public StorageManagerDto getStorageManager(Context context) throws EntityNotFoundException {
        Manager manager = (Manager) traverser.traverse(context);
        if (!manager.isStorageManager()) { // TODO workaround
            throw new EntityNotFoundException("");
        }
        return map(manager);
    }

    private StorageManagerDto map(Manager manager) {
        return StorageManagerDto.newBuilder()
                .id(manager.getId())
                .name(manager.getName())
                .modified(manager.getModified())
                .health(manager.getHealth())
                .healthRollup(manager.getHealthRollup())
                .state(manager.getState())
                .graphicalConsole(translateConsole(manager.getGraphicalConsole()))
                .serialConsole(translateConsole(manager.getSerialConsole()))
                .commandShell(translateConsole(manager.getCommandShell()))
                .currentFirmwareVersion(manager.getFirmwareVersion())
                .managerType(manager.getType())
                .remoteTargets(asRemoteTargetContexts(manager.getManagedRemoteTargets()))
                .services(asStorageServicesContexts(manager.getManagedServices()))
                .logicalDrives(asLogicalDriveContexts(manager.getManagedLogicalDrives()))
                .build();
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
