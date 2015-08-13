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

import com.intel.rsa.podm.business.dto.BladeDto;
import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.CollectionDtoType;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.ComputeModule;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;
import com.intel.rsa.podm.business.services.helpers.BladeDtoHelper;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Collection;

import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;
import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class BladeServiceImpl implements BladeService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getBladeCollection(Context context) throws EntityNotFoundException {
        ComputeModule computeModule = (ComputeModule) traverser.traverse(context);

        Collection<Blade> bladeCollection = computeModule.getBlades();

        return new CollectionDto(
                CollectionDtoType.Blade,
                computeModule.getModified(), // TODO: get the latest date from child, or parent if no child present
                getAsIdList(bladeCollection)
        );
    }

    @Override
    public BladeDto getBlade(Context context) throws EntityNotFoundException {
        Blade blade = (Blade) traverser.traverse(context);
        return map(context, blade);
    }

    private BladeDto map(Context context, Blade blade) {
        BladeDtoHelper bladeDtoHelper = new BladeDtoHelper();


        BladeDto.Builder builder = BladeDto
                .newBuilder()
                .id(blade.getId())
                .name(blade.getName())
                .location(blade.getLocation())
                .modified(blade.getModified())
                .enumStatus(blade.getEnumStatus())
                .manufacturer(blade.getManufacturer())
                .model(blade.getModel())
                .serialNumber(blade.getSerialNumber())
                .health(blade.getHealth())
                .healthRollup(blade.getHealthRollup())
                .state(blade.getState())
                .currentBiosVersion(blade.getCurrentBiosVersion())
                .power(blade.getPower())
                .storageCapable(blade.isStorageCapable())
                .storageControllersCount(blade.getStorageControllerCount())
                .systemType(blade.getSystemType())

                .memoryHealth(blade.getMemoryHealth())
                .memoryHealthRollup(blade.getMemoryHealthRollup())
                .memoryState(blade.getMemoryState())
                .totalSystemMemoryGB(blade.getTotalSystemMemoryGB())
                .memorySockets(blade.getMemorySockets())
                .boot(bladeDtoHelper.buildBoot(blade.getBoot()))

                .processorCount(blade.getProcessorsCount())
                .processorSockets(blade.getProcessorSockets())
                .processorFamily(blade.getProcessorsFamily())
                .processorHealth(blade.getProcessorsHealth())
                .processorHealthRollup(blade.getProcessorsHealthRollup())
                .processorState(blade.getProcessorsState())

                .context(context)
                .containedBy(context.getParent())

                .managedBy(asManagerContexts(blade.getManagers()));

        return builder.build();
    }
}
