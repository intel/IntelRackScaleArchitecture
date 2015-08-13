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

import com.intel.rsa.common.types.Id;
import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.CollectionDtoType;
import com.intel.rsa.podm.business.dto.ProcessorDto;
import com.intel.rsa.podm.business.dto.ProcessorIdDto;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.Processor;
import com.intel.rsa.podm.business.entities.assets.properties.ProcessorId;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Collection;
import java.util.Collections;


@Transactional(value = Transactional.TxType.REQUIRED)
public class ProcessorServiceImpl implements ProcessorService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getProcessorCollection(Context context) throws EntityNotFoundException {
        Blade blade = (Blade) traverser.traverse(context);

        Collection<Processor> processorCollection = blade.getProcessors();

        return new CollectionDto(
                CollectionDtoType.Processor,
                blade.getModified(), // TODO: get the latest date from child, or parent if no child present
                Contexts.getAsIdList(processorCollection)
        );
    }


    @Override
    public ProcessorDto getProcessor(Context context) throws EntityNotFoundException {
        Processor processor = (Processor) traverser.traverse(context);
        return map(context, processor);
    }

    private ProcessorDto map(Context context, Processor processor) {
        return ProcessorDto
                .newBuilder()
                .id(processor.getId())
                .name(processor.getName())
                .location(processor.getLocation())
                .modified(processor.getModified())
                .socket(processor.getSocket())
                .manufacturer(processor.getManufacturer())
                .model(processor.getModel())
                .processorType(processor.getProcessorType())
                .processorArchitecture(processor.getProcessorArchitecture())
                .instructionSet(processor.getInstructionSet())
                .instructionSetExtensions(processor.getInstructionSetExtensions())
                .processorId(buildProcessorId(processor.getProcessorId()))
                .maxSpeedMHz(processor.getMaxSpeedMhz())
                .totalCores(processor.getTotalCores())
                .enabledCores(processor.getEnabledCores())
                .totalThreads(processor.getTotalThreads())
                .enabledThreads(processor.getEnabledThreads())
                .health(processor.getHealth())
                .healthRollup(processor.getHealthRollup())
                .state(processor.getState())
                .managedBy(Collections.<Id>emptyList())
                .context(context)
                .build();
    }

    private ProcessorIdDto buildProcessorId(ProcessorId processorId) {
        //Since this value is optional it can be null
        if (processorId == null) {
            return null;
        }

        return ProcessorIdDto
                .newBuilder()
                .vendorId(processorId.getVendorId())
                .numericId(processorId.getNumericId())
                .family(processorId.getFamily())
                .model(processorId.getModel())
                .step(processorId.getStep())
                .microcodeInfo(processorId.getMicrocodeInfo())
                .build();
    }
}
