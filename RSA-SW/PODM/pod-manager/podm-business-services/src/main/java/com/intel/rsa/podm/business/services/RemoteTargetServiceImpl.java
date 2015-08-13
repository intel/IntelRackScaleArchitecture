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
import com.intel.rsa.podm.business.dto.RemoteTargetDto;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.assets.StorageService;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;
import com.intel.rsa.podm.business.services.helpers.RemoteTargetDtoMapHelper;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Collection;

import static com.intel.rsa.podm.business.services.Contexts.asLogicalDriveContexts;
import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;
import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class RemoteTargetServiceImpl implements RemoteTargetService {

    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getRemoteTargetCollection(Context context) throws EntityNotFoundException {
        StorageService storageService = (StorageService) traverser.traverse(context);

        Collection<RemoteTarget> targetCollection = storageService.getRemoteTargets();

        return new CollectionDto(
                CollectionDtoType.Target,
                storageService.getModified(),
                getAsIdList(targetCollection));
    }

    @Override
    public RemoteTargetDto getRemoteTarget(Context context) throws EntityNotFoundException {
        RemoteTarget target = (RemoteTarget) traverser.traverse(context);
        return map(context, target);
    }

    private RemoteTargetDto map(Context context, RemoteTarget target) {
        RemoteTargetDtoMapHelper remoteTargetDtoMapHelper = new RemoteTargetDtoMapHelper();

        RemoteTargetDto.Builder builder = RemoteTargetDto.newBuilder()
                .id(target.getId())
                .name(target.getName())
                .modified(target.getModified())
                .state(target.getState())
                .health(target.getHealth())
                .healthRollup(target.getHealthRollup())
                .enumStatus(target.getEnumStatus())
                .type(target.getType())
                .context(context)
                .iscsiAddresses(remoteTargetDtoMapHelper.extractIscsiAddresses(target))
                .initiators(remoteTargetDtoMapHelper.extractIscsiInitiators(target))
                .logicalDrives(asLogicalDriveContexts(target.getLogicalDrives()))
                .managedby(asManagerContexts(target.getManagers()));

        return builder.build();
    }
}
