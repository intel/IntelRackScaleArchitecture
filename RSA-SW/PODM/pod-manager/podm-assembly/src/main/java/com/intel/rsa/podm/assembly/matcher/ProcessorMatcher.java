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

package com.intel.rsa.podm.assembly.matcher;

import com.intel.rsa.common.types.InstructionSet;
import com.intel.rsa.common.types.InstructionSetExtension;
import com.intel.rsa.common.types.ProcessorModel;
import com.intel.rsa.podm.assembly.AllocationException;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.dto.RequestedProcessor;
import com.intel.rsa.podm.business.entities.assets.Processor;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.stream.Collectors;

public class ProcessorMatcher {
    public boolean matches(RequestedNode template, Collection<Processor> processors) {
        if (template.getProcessors() != null && !template.getProcessors().isEmpty()) {
            return isTemplateProcessorMatchesProcessors(template.getProcessors().get(0), processors);
        }

        return !processors.isEmpty();
    }

    public void validate(RequestedNode template) throws AllocationException {
        if (template.getProcessors() != null && template.getProcessors().size() > 1) {
            throw new AllocationException("Allocation of more than one processor type is not supported");
        }
    }

    private boolean isTemplateProcessorMatchesProcessors(RequestedProcessor templateProcessor, Collection<Processor> processors) {
        List<Processor> localFilteredProcessors = new ArrayList<>(processors);

        localFilteredProcessors = filterProcessorByModel(localFilteredProcessors, templateProcessor.getModel());
        localFilteredProcessors = filterProcessorByInstructionSet(localFilteredProcessors, templateProcessor.getInstructionSet());
        localFilteredProcessors = filterProcessorByInstructionSetExtension(localFilteredProcessors, templateProcessor.getInstructionSetExtensions());
        localFilteredProcessors = filterProcessorByCores(localFilteredProcessors, templateProcessor.getTotalCores());
        localFilteredProcessors = filterProcessorByMaxSpeed(localFilteredProcessors, templateProcessor.getMaxSpeedMHz());

        if (templateProcessor.getQuantity() != null) {
            return localFilteredProcessors.size() >= templateProcessor.getQuantity();
        } else {
            return !localFilteredProcessors.isEmpty();
        }
    }

    private List<Processor> filterProcessorByModel(List<Processor> processors, ProcessorModel processorModel) {
        if (processorModel == null) {
            return processors;
        }

        return processors.stream()
                .filter(processor -> processor.getModel() == processorModel)
                .collect(Collectors.toList());
    }

    private List<Processor> filterProcessorByInstructionSet(List<Processor> processors, InstructionSet instructionSet) {
        if (instructionSet == null) {
            return processors;
        }

        return processors.stream()
                .filter(processor -> processor.getInstructionSet() == instructionSet)
                .collect(Collectors.toList());
    }

    private List<Processor> filterProcessorByInstructionSetExtension(List<Processor> processors, List<InstructionSetExtension> instructionSetExtensions) {
        if (instructionSetExtensions == null) {
            return processors;
        }

        return processors.stream()
                .filter(processor -> processor.getInstructionSetExtensions().toList().containsAll(instructionSetExtensions))
                .collect(Collectors.toList());
    }

    private List<Processor> filterProcessorByCores(List<Processor> processors, Integer totalCores) {
        if (totalCores == null) {
            return processors;
        }

        return processors.stream()
                .filter(processor -> processor.getTotalCores() >= totalCores)
                .collect(Collectors.toList());
    }

    private List<Processor> filterProcessorByMaxSpeed(List<Processor> processors, Integer maxSpeedMHz) {
        if (maxSpeedMHz == null) {
            return processors;
        }

        return processors.stream()
                .filter(processor -> processor.getMaxSpeedMhz() >= maxSpeedMHz)
                .collect(Collectors.toList());
    }
}
