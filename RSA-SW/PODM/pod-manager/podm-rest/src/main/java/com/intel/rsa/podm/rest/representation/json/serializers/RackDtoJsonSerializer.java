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

import com.intel.rsa.podm.business.dto.RackAttributesDto;
import com.intel.rsa.podm.business.dto.RackDto;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.RackJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.RackAttributesJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.TrayPresentJson;

import java.util.stream.Collectors;

import static com.intel.rsa.podm.business.services.context.ContextType.DRAWER;
import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfCollectionInContext;
import static java.net.URI.create;

public class RackDtoJsonSerializer extends DtoJsonSerializer<RackDto> {
    public RackDtoJsonSerializer() {
        super(RackDto.class);
    }

    @Override
    protected RackJson translate(RackDto dto) {

        RackJson result = new RackJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAPods/Links/Members/1/Links/Racks/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.location = toJson(dto.getLocation());
        mapStatus(dto, result);
        result.modified = dto.getModified();
        result.links.drawers = oDataIdOfCollectionInContext(dto.getContext(), DRAWER);

        result.links.managedBy.addAll(dto.getManagedBy().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));

        appendRackAttributes(result, dto.getAttributes());
        return result;
    }

    private void mapStatus(RackDto dto, RackJson result) {
        result.status.state = dto.getState();
        result.status.health = dto.getHealth();
    }

    private void appendRackAttributes(RackJson rackJson, RackAttributesDto rackAttributesDto) {
        RackAttributesJson result = new RackAttributesJson();

        result.isRmmPresent = rackAttributesDto.isRmmPresent();
        result.trayPresent.trays.addAll(rackAttributesDto.getTraysPresent().stream()
                .map(rackTray -> new TrayPresentJson.Tray(rackTray.getTrayRuid()))
                .collect(Collectors.toList()));

        rackJson.rackAttributes = result;
    }
}

