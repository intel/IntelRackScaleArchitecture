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
import com.intel.rsa.podm.business.dto.MemoryDto;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.Memory;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class MemoryServiceImpl implements MemoryService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getMemoryCollection(Context context) throws EntityNotFoundException {
        Blade blade = (Blade) traverser.traverse(context);

        return new CollectionDto(CollectionDtoType.Memory, blade.getModified(), getAsIdList(blade.getMemory()));
    }

    @Override
    public MemoryDto getMemory(Context context) throws EntityNotFoundException {
        Memory memory = (Memory) traverser.traverse(context);

        return MemoryDto
                .newBuilder()
                .id(memory.getId())
                .name(memory.getName())
                .context(context)
                .modified(memory.getModified())
                .manufacturer(memory.getManufacturer())
                .socket(memory.getSocket())
                .bank(memory.getBank())
                .type(memory.getType())
                .sizeGB(memory.getSizeGB())
                .speedMHz(memory.getSpeedMHz())
                .voltageVolt(memory.getVoltageVolt())
                .dataWidthBits(memory.getDataWidthBits())
                .totalWidthBits(memory.getTotalWidthBits())
                .formFactor(memory.getFormFactor())
                .serialNumber(memory.getSerialNumber())
                .assetTag(memory.getAssetTag())
                .partNumber(memory.getPartNumber())
                .rank(memory.getRank())
                .configuredSpeedMHz(memory.getConfiguredSpeedMhz())
                .minimumVoltageVolt(memory.getMinimumVoltageVolt())
                .maximumVoltageVolt(memory.getMaximumVoltageVolt())
                .state(memory.getState())
                .health(memory.getHealth())
                .location(memory.getLocation())
                .build();
    }

}
