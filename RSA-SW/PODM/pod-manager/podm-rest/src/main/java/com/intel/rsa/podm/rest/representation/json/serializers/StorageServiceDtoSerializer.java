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

import com.intel.rsa.podm.business.dto.StorageServiceDto;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.StorageServiceJson;

import java.util.stream.Collectors;

import static com.intel.rsa.podm.business.services.context.ContextType.LOGICAL_DRIVE;
import static com.intel.rsa.podm.business.services.context.ContextType.PHYSICAL_DRIVE;
import static com.intel.rsa.podm.business.services.context.ContextType.REMOTE_TARGET;
import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfCollectionInContext;
import static java.net.URI.create;

public class StorageServiceDtoSerializer extends DtoJsonSerializer<StorageServiceDto> {
    public StorageServiceDtoSerializer() {
        super(StorageServiceDto.class);
    }

    @Override
    protected StorageServiceJson translate(StorageServiceDto dto) {
        StorageServiceJson result = new StorageServiceJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAStorageServices/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.modified = dto.getModified();

        result.status.state = dto.getState();
        result.status.health = dto.getHealth();
        result.status.healthRollup = dto.getHealthRollup();

        result.enumStatus = dto.getEnumStatus();
        result.location = toJson(dto.getLocation());
        result.links.remoteTargets = oDataIdOfCollectionInContext(dto.getContext(), REMOTE_TARGET);
        result.links.drives =  oDataIdOfCollectionInContext(dto.getContext(), PHYSICAL_DRIVE);
        result.links.logicalDrives =  oDataIdOfCollectionInContext(dto.getContext(), LOGICAL_DRIVE);
        result.links.managedBy.addAll(dto.getManagedBy().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));

        return result;
    }
}
