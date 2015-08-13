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

package com.intel.rsa.podm.discovery.external.mapper.psme;

import com.intel.rsa.client.api.reader.psme.PsmeProcessor;
import com.intel.rsa.client.api.reader.psme.PsmeProcessorId;
import com.intel.rsa.podm.business.entities.assets.Processor;
import com.intel.rsa.podm.business.entities.assets.properties.ProcessorId;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.discovery.external.mapper.DomainObjectMapper;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;

@Dependent
public class ProcessorMapper extends DomainObjectMapper<PsmeProcessor, Processor> {
    @Inject
    private DomainObjectRepository repository;

    protected ProcessorMapper() {
        super(PsmeProcessor.class, Processor.class);
        registerProvider(ProcessorId.class, this::provideProcessorId);
    }

    private ProcessorId provideProcessorId(PsmeProcessorId psmeProcessorId) {
        ProcessorId processorId = target.getProcessorId();
        if (processorId == null) {
            processorId = repository.create(ProcessorId.class);
        }
        return processorId;
    }

    @Override
    protected void performNotAutomatedMapping(PsmeProcessor source, Processor target) {
        super.performNotAutomatedMapping(source, target);
        target.updateModified();
    }
}
