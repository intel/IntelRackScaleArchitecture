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
import com.intel.rsa.podm.business.dto.LogicalDriveDto;
import com.intel.rsa.podm.business.entities.assets.LogicalDrive;
import com.intel.rsa.podm.business.entities.assets.StorageService;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.services.Contexts.asLogicalDriveContexts;
import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;
import static com.intel.rsa.podm.business.services.Contexts.asPhysicalDriveContexts;
import static com.intel.rsa.podm.business.services.Contexts.asRemoteTargetContexts;
import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class LogicalDriveServiceImpl implements LogicalDriveService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getLogicalDriveCollection(Context context) throws EntityNotFoundException {
        StorageService storageService = (StorageService) traverser.traverse(context);
        return new CollectionDto(
                CollectionDtoType.LogicalDrive,
                storageService.getModified(),
                getAsIdList(storageService.getLogicalDrives())
            );
    }

    @Override
    public LogicalDriveDto getLogicalDrive(Context context) throws EntityNotFoundException {
        LogicalDrive drive = (LogicalDrive) traverser.traverse(context);
        return LogicalDriveDto.newBuilder()
                .id(drive.getId())
                .name(drive.getName())
                .modified(drive.getModified())
                .state(drive.getState())
                .health(drive.getHealth())
                .healthRollup(drive.getHealthRollup())
                .type(drive.getType())
                .mode(drive.getMode())
                .writeProtected(drive.getWriteProtected())
                .snapshot(drive.getSnapshot())
                .capacityGB(drive.getCapacityGb())
                .assetTag(drive.getAssetTag())
                .image(drive.getImage())
                .bootable(drive.getBootable())
                .context(context)
                .physicalDrives(asPhysicalDriveContexts(drive.getPhysicalDrives()))
                .masterDrive(Contexts.toContext(drive.getMasterDrive()))
                .logicalDrives(asLogicalDriveContexts(drive.getUsedLogicalDrives()))
                .usedBy(asLogicalDriveContexts(drive.getUsedBy()))
                .targets(asRemoteTargetContexts(drive.getRemoteTargets()))
                .managedBy(asManagerContexts(drive.getManagers()))
                .build();
    }
}
