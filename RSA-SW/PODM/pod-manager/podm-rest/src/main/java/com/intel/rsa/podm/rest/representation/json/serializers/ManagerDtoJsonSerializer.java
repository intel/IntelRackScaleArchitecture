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

import com.intel.rsa.podm.business.dto.ConsoleDto;
import com.intel.rsa.podm.business.dto.ManagerDto;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.ManagerJson;

import java.util.stream.Collectors;

import static com.intel.rsa.podm.business.services.context.ContextType.NETWORK_INTERFACE;
import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfCollectionInContext;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfResourceInContext;
import static com.intel.rsa.podm.rest.resources.ResourceNames.NETWORK_SERVICE_RESOURCE_NAME;
import static java.net.URI.create;

public final class ManagerDtoJsonSerializer extends DtoJsonSerializer<ManagerDto> {
    public ManagerDtoJsonSerializer() {
        super(ManagerDto.class);
    }

    @Override
    protected ManagerJson translate(ManagerDto dto) {
        ManagerJson result = new ManagerJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAManagers/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.modified = dto.getModified();
        result.managerType = dto.getManagerType();
        result.model = dto.getModel();
        mapStatus(dto, result);

        fillConsoleJson(result.graphicalConsole, dto.getGraphicalConsole());
        fillConsoleJson(result.serialConsole, dto.getSerialConsole());
        fillConsoleJson(result.commandShell, dto.getCommandShell());

        result.firmware.current.versionString = dto.getCurrentFirmwareVersion();

        fillLinks(result, dto);

        return result;
    }

    private void mapStatus(ManagerDto dto, ManagerJson result) {
        result.status.health = dto.getHealth();
        result.status.state = dto.getState();
        result.status.healthRollup = dto.getHealthRollup();
    }

    private void fillConsoleJson(ManagerJson.Console console, ConsoleDto consoleDto) {
        console.enabled = consoleDto.isEnabled();
        console.maxConcurrentSessions = consoleDto.getMaxConcurrentSessions();
        console.connectTypesSupported = consoleDto.getConnectTypesSupported();
    }

    private void fillLinks(ManagerJson json, ManagerDto dto) {
        json.links.networkService = oDataIdOfResourceInContext(dto.getContext(), NETWORK_SERVICE_RESOURCE_NAME);
        json.links.simpleNetwork = oDataIdOfCollectionInContext(dto.getContext(), NETWORK_INTERFACE);
        json.links.managerForBlades.addAll(dto.getManagerForBlades().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        json.links.managerForComputeModules.addAll(dto.getManagerForComputeModules().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        json.links.managerForFabricModules.addAll(dto.getManagerForFabricModules().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        json.links.managerForDrawers.addAll(dto.getManagerForDrawers().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        json.links.managerForSwitches.addAll(dto.getManagerForSwitches().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
    }
}
