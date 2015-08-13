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

import com.intel.rsa.podm.business.dto.ComposedNodeDto;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.ComposedNodeJson;

import java.util.stream.Collectors;

import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static java.net.URI.create;

public class ComposedNodeDtoJsonSerializer extends DtoJsonSerializer<ComposedNodeDto> {

    public ComposedNodeDtoJsonSerializer() {
        super(ComposedNodeDto.class);
    }

    @Override
    protected ComposedNodeJson translate(ComposedNodeDto dto) {
        ComposedNodeJson result = new ComposedNodeJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAComposedNodes/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.description = dto.getDescription();
        result.modified = dto.getModified();
        result.systemType = dto.getSystemType();
        result.composedNodeState = dto.getComposedNodeState();

        appendProcessorSummary(dto, result);
        appendMemorySummary(dto, result);
        mapState(dto, result);
        collectLinks(dto, result);

        result.actions.resetAction.target = result.oDataId + "/Actions/RSAComposedNode.Reset";
        result.actions.assembleAction.target = result.oDataId + "/Actions/RSAComposedNode.Assemble";

        return result;
    }

    private void mapState(ComposedNodeDto dto, ComposedNodeJson result) {
        result.status.state = dto.getStatus().getState();
        result.status.health = dto.getStatus().getHealth();
        result.status.healthRollup = dto.getStatus().getHealthRollup();
    }

    private void appendProcessorSummary(ComposedNodeDto dto, ComposedNodeJson result) {
        result.processors.count = dto.getProcessors().getCount();
        result.processors.processorSockets = dto.getProcessors().getProcessorSockets();
        result.processors.processorFamily = dto.getProcessors().getProcessorFamily();
        result.processors.status.state = dto.getProcessors().getStatus().getState();
        result.processors.status.health = dto.getProcessors().getStatus().getHealth();
        result.processors.status.healthRollup = dto.getProcessors().getStatus().getHealthRollup();
    }

    private void appendMemorySummary(ComposedNodeDto dto, ComposedNodeJson result) {
        result.memory.totalSystemMemoryGB = dto.getMemory().getTotalSystemMemoryGB();
        result.memory.memorySockets = dto.getMemory().getMemorySockets();
        result.memory.status.state = dto.getMemory().getStatus().getState();
        result.memory.status.health = dto.getMemory().getStatus().getHealth();
        result.memory.status.healthRollup = dto.getMemory().getStatus().getHealthRollup();
    }

    private void collectLinks(ComposedNodeDto dto, ComposedNodeJson result) {
        result.links.processors.addAll(dto.getProcessorCollection().stream().map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        result.links.memory.addAll(dto.getMemoryCollection().stream().map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        result.links.remoteDrives.addAll(dto.getRemoteDriveCollection().stream().map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        result.links.localDrives.addAll(dto.getLocalDriveCollection().stream().map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        result.links.ethernetInterfaces.addAll(dto.getEthernetInterfaceCollection().stream().map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        result.links.managedBy.addAll(dto.getManagedByCollection().stream().map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
    }
}
