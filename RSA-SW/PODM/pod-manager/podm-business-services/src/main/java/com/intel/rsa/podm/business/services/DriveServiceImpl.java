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
import com.intel.rsa.podm.business.dto.DriveDto;
import com.intel.rsa.podm.business.entities.assets.Drive;
import com.intel.rsa.podm.business.entities.assets.StorageController;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class DriveServiceImpl implements DriveService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getDriveCollection(Context context) throws EntityNotFoundException {
        StorageController storageController = (StorageController) traverser.traverse(context);
        return new CollectionDto(
                CollectionDtoType.Drive,
                storageController.getModified(),
                getAsIdList(storageController.getDrives())
                );
    }

    @Override
    public DriveDto getDrive(Context context) throws EntityNotFoundException {
        Drive drive = (Drive) traverser.traverse(context);
        return DriveDto.newBuilder()
                .id(drive.getId())
                .state(drive.getState())
                .health(drive.getHealth())
                .healthRollup(drive.getHealthRollup())
                .capacityGB(drive.getCapacityGb())
                .controllerInterface(drive.getControllerInterface())
                .manufacturer(drive.getManufacturer())
                .model(drive.getModel())
                .serialNumber(drive.getSerialNumber())
                .location(drive.getLocation())
                .modified(drive.getModified())
                .name(drive.getName())
                .rpm(drive.getRpm())
                .type(drive.getType())
                .context(context)
                .build();
    }
}
