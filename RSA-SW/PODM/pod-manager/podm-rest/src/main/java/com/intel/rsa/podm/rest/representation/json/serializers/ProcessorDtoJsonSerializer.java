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

package com.intel.rsa.podm.rest.representation.json.serializers;

import com.intel.rsa.common.types.InstructionSet;
import com.intel.rsa.podm.business.dto.ProcessorDto;
import com.intel.rsa.podm.business.dto.ProcessorIdDto;
import com.intel.rsa.podm.rest.representation.json.templates.ProcessorJson;

import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdFromContext;
import static java.net.URI.create;

public class ProcessorDtoJsonSerializer extends DtoJsonSerializer<ProcessorDto> {

    public ProcessorDtoJsonSerializer() {
        super(ProcessorDto.class);
    }

    @Override
    protected ProcessorJson translate(ProcessorDto dto) {
        ProcessorJson result = new ProcessorJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAProcessors/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());
        result.id = dto.getId();
        result.name = dto.getName();
        result.location = toJson(dto.getLocation());
        result.modified = dto.getModified();
        result.socket = dto.getSocket();
        result.manufacturer = dto.getManufacturer();
        result.model = dto.getModel();
        result.processorType = dto.getProcessorType();
        result.processorArchitecture = dto.getProcessorArchitecture();
        result.instructionSet = serializeInstructionSet(dto.getInstructionSet());
        result.instructionSetExtensions.addAll(dto.getInstructionSetExtension().toStringList());
        result.processorId = translateProcessorId(dto.getProcessorId());
        result.maxSpeedMHz = dto.getMaxSpeedMHz();
        result.totalCores = dto.getTotalCores();
        result.enabledCores = dto.getEnabledCores();
        result.totalThreads = dto.getTotalThreads();
        result.enabledThreads = dto.getEnabledThreads();
        mapStatus(dto, result);
        result.links.containedBy = oDataIdFromContext(dto.getContext().getParent());
        return result;
    }

    private void mapStatus(ProcessorDto dto, ProcessorJson result) {
        result.status.state = dto.getState();
        result.status.health = dto.getHealth();
        result.status.healthRollup = dto.getHealthRollup();
    }

    private String serializeInstructionSet(InstructionSet instructionSet) {
        //Since is optional value it can be null
        return instructionSet == null ? null : instructionSet.getValue();
    }

    private ProcessorJson.ProcessorId translateProcessorId(ProcessorIdDto dto) {
        //Since is optional value it can be null
        if (dto == null) {
            return null;
        }

        ProcessorJson.ProcessorId processorId  = new ProcessorJson.ProcessorId();
        processorId.vendorId = dto.getVendorId();
        processorId.numericId = dto.getNumericId();
        processorId.family = dto.getFamily();
        processorId.model = dto.getModel();
        processorId.step = dto.getStep();
        processorId.microcodeInfo = dto.getMicrocodeInfo();

        return processorId;
    }
}
