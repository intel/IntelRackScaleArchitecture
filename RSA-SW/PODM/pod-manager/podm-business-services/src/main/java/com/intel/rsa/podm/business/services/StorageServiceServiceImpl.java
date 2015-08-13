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
import com.intel.rsa.podm.business.dto.StorageServiceDto;
import com.intel.rsa.podm.business.entities.assets.StorageService;
import com.intel.rsa.podm.business.entities.assets.StorageServiceCollection;
import com.intel.rsa.podm.business.entities.assets.StorageServiceCollectionProvider;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;
import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class StorageServiceServiceImpl implements StorageServiceService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Inject
    private StorageServiceCollectionProvider provider;

    @Override
    public CollectionDto getStorageServiceCollection() {
        StorageServiceCollection storageServiceCollection = provider.getInstance();

        return new CollectionDto(
                CollectionDtoType.StorageService,
                storageServiceCollection.getModified(),
                getAsIdList(storageServiceCollection.getMembers()));
    }

    @Override
    public StorageServiceDto getStorageService(Context context) throws EntityNotFoundException {
        StorageService storageService = (StorageService) traverser.traverse(context);
        return map(context, storageService);
    }

    private StorageServiceDto map(Context context, StorageService storageService) {
        return StorageServiceDto
                .newBuilder()
                .id(storageService.getId())
                .name(storageService.getName())
                .context(context)
                .modified(storageService.getModified())
                .state(storageService.getState())
                .health(storageService.getHealth())
                .healthRollup(storageService.getHealthRollup())
                .enumStatus(storageService.getEnumStatus())
                .location(storageService.getLocation())
                .managedBy(asManagerContexts(storageService.getManagers()))
                .build();
    }
}
