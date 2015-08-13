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

import com.intel.rsa.podm.business.dto.BladeDto;
import com.intel.rsa.podm.business.dto.BootDto;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.BladeJson;

import java.util.stream.Collectors;

import static com.intel.rsa.podm.business.services.context.ContextType.MEMORY;
import static com.intel.rsa.podm.business.services.context.ContextType.NETWORK_INTERFACE;
import static com.intel.rsa.podm.business.services.context.ContextType.PROCESSOR;
import static com.intel.rsa.podm.business.services.context.ContextType.STORAGE_CONTROLLER;
import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfCollectionInContext;
import static java.net.URI.create;

public class BladeDtoJsonSerializer extends DtoJsonSerializer<BladeDto> {
    public BladeDtoJsonSerializer() {
        super(BladeDto.class);
    }

    @Override
    protected BladeJson translate(BladeDto dto) {
        BladeJson result = new BladeJson();
        result.oDataContext = create("/rest/v1/$$metadata#RSABlades");
        result.oDataId = oDataId(context.getRequestPath());
        result.id = dto.getId();
        result.name = dto.getName();
        result.location = toJson(dto.getLocation());
        result.modified = dto.getModified();
        result.manufacturer = dto.getManufacturer();
        result.model = dto.getModel();
        result.serialNumber = dto.getSerialNumber();
        mapStatus(dto, result);
        result.systemType = dto.getSystemType();
        result.storageCapable = dto.isStorageCapable();
        result.storageControllersCount = dto.getStorageControllersCount();
        result.power = dto.getPower();
        result.enumStatus = dto.getEnumStatus();
        result.bios.current.versionString = dto.getCurrentBiosVersion();
        mapProcessors(dto, result);
        mapMemory(dto, result);
        result.boot = translateBoot(dto.getBoot());
        constructLinks(dto, result);
        result.actions.resetAction.target = result.oDataId + "/Actions/RSABlade.Reset"; //todo set resetAction.restTypes[] as obtained from psme blade
        return result;
    }

    private void mapStatus(BladeDto dto, BladeJson result) {
        result.status.state = dto.getState();
        result.status.health = dto.getHealth();
        result.status.healthRollup = dto.getHealthRollup();
    }

    private void mapMemory(BladeDto dto, BladeJson result) {
        result.memory.totalSystemMemoryGB = dto.getTotalSystemMemoryGB();
        result.memory.memorySockets = dto.getMemorySockets();
        result.memory.status.state = dto.getMemoryState();
        result.memory.status.healthRollup = dto.getMemoryHealthRollup();
        result.memory.status.health = dto.getMemoryHealth();
    }

    private void mapProcessors(BladeDto dto, BladeJson result) {
        result.processors.count = dto.getProcessorCount();
        result.processors.processorSockets = dto.getProcessorSockets();
        result.processors.processorFamily = dto.getProcessorFamily();
        result.processors.status.health = dto.getProcessorHealth();
        result.processors.status.healthRollup = dto.getMemoryHealthRollup();
        result.processors.status.state = dto.getProcessorState();
    }

    private void constructLinks(BladeDto dto, BladeJson result) {
        result.links.processors = oDataIdOfCollectionInContext(dto.getContext(), PROCESSOR);
        result.links.memory = oDataIdOfCollectionInContext(dto.getContext(), MEMORY);
        result.links.storageControllers = oDataIdOfCollectionInContext(dto.getContext(), STORAGE_CONTROLLER);
        result.links.ethernetInterfaces = oDataIdOfCollectionInContext(dto.getContext(), NETWORK_INTERFACE);

        result.links.containedBy.addAll(dto.getContainedBy().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        result.links.managedBy.addAll(dto.getManagedBy().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
    }

    private BladeJson.Boot translateBoot(BootDto dto) {
        //Since is optional value it can be null
        if (dto == null) {
            return null;
        }

        BladeJson.Boot boot  = new BladeJson.Boot();
        boot.bootSourceOverrideEnabled = dto.getBootSourceOverrideEnabled();
        boot.bootSourceOverrideTarget = dto.getBootSourceOverrideTarget();
        boot.bootSourceOverrideSupported.addAll(dto.getBootSourceOverrideSupported());
        boot.uefiTargetBootSourceOverride = dto.getUefiTargetBootSourceOverride();

        return boot;
    }
}
