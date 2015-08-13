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
import com.intel.rsa.podm.business.dto.ComputeModuleDto;
import com.intel.rsa.podm.business.entities.assets.ComputeModule;
import com.intel.rsa.podm.business.entities.assets.Drawer;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Collection;

import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;
import static java.util.Collections.singletonList;

@Transactional(value = Transactional.TxType.REQUIRED)
public class ComputeModuleServiceImpl implements ComputeModuleService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getComputeModuleCollection(Context context) throws EntityNotFoundException {
        Drawer drawer = (Drawer) traverser.traverse(context);

        Collection<ComputeModule> computeModuleCollection = drawer.getComputeModules();

        return new CollectionDto(
                CollectionDtoType.ComputeModule,
                drawer.getModified(), // TODO: get the latest date from child, or parent if no child present
                Contexts.getAsIdList(computeModuleCollection)
        );
    }

    @Override
    public ComputeModuleDto getComputeModule(Context context) throws EntityNotFoundException {
        ComputeModule computeModule = (ComputeModule) traverser.traverse(context);
        return map(context, computeModule);
    }

    private ComputeModuleDto map(Context context, ComputeModule computeModule) {
        return ComputeModuleDto.newBuilder()
                .id(computeModule.getId())
                .name(computeModule.getName())
                .location(computeModule.getLocation())
                .modified(computeModule.getModified())
                .manufacturer(computeModule.getManufacturer())
                .model(computeModule.getModel())
                .serialNumber(computeModule.getSerialNumber())
                .health(computeModule.getHealth())
                .healthRollup(computeModule.getHealthRollup())
                .state(computeModule.getState())
                .context(context)
                .enumStatus(computeModule.getEnumStatus())
                .managedBy(asManagerContexts(computeModule.getManagers()))
                .containedBy(singletonList(context.getParent()))
                .build();
    }
}
