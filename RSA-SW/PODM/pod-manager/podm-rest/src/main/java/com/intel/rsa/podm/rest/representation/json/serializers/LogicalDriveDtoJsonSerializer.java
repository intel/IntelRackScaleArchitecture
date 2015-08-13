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

import com.intel.rsa.podm.business.dto.LogicalDriveDto;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.LogicalDriveJson;

import java.util.stream.Collectors;

import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdFromContext;
import static java.net.URI.create;

public class LogicalDriveDtoJsonSerializer extends DtoJsonSerializer<LogicalDriveDto> {
    public LogicalDriveDtoJsonSerializer() {
        super(LogicalDriveDto.class);
    }

    @Override
    protected LogicalDriveJson translate(LogicalDriveDto dto) {
        LogicalDriveJson result = new LogicalDriveJson();

        result.oDataContext = create("/rest/v1/$metadata#RSALogicalDrives/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.modified = dto.getModified();
        result.type = dto.getType();
        result.mode = dto.getMode();
        result.writeProtected = dto.isWriteProtected();
        result.snapshot = dto.getSnapshot();
        result.assetTag = dto.getAssetTag();
        result.capacityGB = dto.getCapacityGB();
        result.image = dto.getImage();
        result.bootable = dto.isBootable();
        mapStatus(dto, result);

        constructLinks(dto, result);

        return result;
    }

    private void constructLinks(LogicalDriveDto dto, LogicalDriveJson result) {
        result.links.logicalDrives.addAll(dto.getLogicalDrives().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));

        result.links.physicalDrives.addAll(dto.getPhysicalDrives().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));

        result.links.masterDrive = oDataIdFromContext(dto.getMasterDrive());

        result.links.usedBy.addAll(dto.getUsedBy().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));

        result.links.targets.addAll(dto.getTargets().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));

        result.links.managedBy.addAll(dto.getManagedBy().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
    }

    private void mapStatus(LogicalDriveDto dto, LogicalDriveJson result) {
        result.status.state = dto.getState();
        result.status.health = dto.getHealth();
        result.status.healthRollup = dto.getHealthRollup();
    }

}
