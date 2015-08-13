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
import com.intel.rsa.podm.business.dto.PhysicalDriveDto;
import com.intel.rsa.podm.business.entities.assets.PhysicalDrive;
import com.intel.rsa.podm.business.entities.assets.StorageService;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.services.Contexts.asLogicalDriveContexts;
import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;
import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class PhysicalDriveServiceImpl implements PhysicalDriveService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getPhysicalDriveCollection(Context context) throws EntityNotFoundException {
        StorageService storageService = (StorageService) traverser.traverse(context);
        return new CollectionDto(
                CollectionDtoType.PhysicalDrive,
                storageService.getModified(),
                getAsIdList(storageService.getPhysicalDrives())
                );
    }

    @Override
    public PhysicalDriveDto getPhysicalDrive(Context context) throws EntityNotFoundException {
        PhysicalDrive drive = (PhysicalDrive) traverser.traverse(context);
        return PhysicalDriveDto.newBuilder()
                .id(drive.getId())
                .state(drive.getState())
                .health(drive.getHealth())
                .healthRollup(drive.getHealthRollup())
                .capacityGB(drive.getCapacityGb())
                .controllerInterface(drive.getControllerInterface())
                .manufacturer(drive.getManufacturer())
                .model(drive.getModel())
                .serialNumber(drive.getSerialNumber())
                .modified(drive.getModified())
                .name(drive.getName())
                .rpm(drive.getRpm())
                .type(drive.getType())
                .context(context)
                .managedBy(asManagerContexts(drive.getManagers()))
                .usedBy(asLogicalDriveContexts(drive.getUsedBy()))
                .build();
    }
}
