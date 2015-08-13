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

import com.intel.rsa.podm.business.dto.FabricModuleDto;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.FabricModuleJson;

import java.util.Collection;
import java.util.stream.Collectors;

import static com.intel.rsa.podm.business.services.context.ContextType.SWITCH;
import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdFromContext;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfCollectionInContext;
import static java.net.URI.create;

public final class FabricModuleDtoSerializer extends DtoJsonSerializer<FabricModuleDto> {
    public FabricModuleDtoSerializer() {
        super(FabricModuleDto.class);
    }

    @Override
    protected FabricModuleJson translate(FabricModuleDto dto) {
        FabricModuleJson result = new FabricModuleJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAFabricModules/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.location = toJson(dto.getLocation());
        result.modified = dto.getModified();

        result.status.state = dto.getState();
        result.status.health = dto.getHealth();

        result.links.switches = oDataIdOfCollectionInContext(dto.getContext(), SWITCH);
        result.links.containedBy = oDataIdFromContext(dto.getContext().getParent());
        result.links.managedBy.addAll(getManagerLinks(dto));

        return result;
    }

    private static Collection<ODataId> getManagerLinks(FabricModuleDto dto) {
        return dto.getManagedBy().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList());
    }
}
