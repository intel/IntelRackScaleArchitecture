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

import com.intel.rsa.podm.business.dto.SwitchPortDto;
import com.intel.rsa.podm.rest.representation.json.templates.SwitchPortJson;

import static com.intel.rsa.podm.business.services.context.ContextType.VLAN;
import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdFromContext;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfCollectionInContext;
import static java.net.URI.create;

public class SwitchPortDtoJsonSerializer extends DtoJsonSerializer<SwitchPortDto> {
    protected SwitchPortDtoJsonSerializer() {
        super(SwitchPortDto.class);
    }

    @Override
    protected SwitchPortJson translate(SwitchPortDto dto) {
        SwitchPortJson result = new SwitchPortJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAPorts/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.location = toJson(dto.getLocation());
        result.modified = dto.getModified();

        setStatus(result, dto);

        result.linkType = dto.getLinkType();
        result.operationalState = dto.getOperationalState();
        result.administrativeState = dto.getAdministrativeState();
        result.linkSpeedGbps = dto.getLinkSpeedGbps();
        result.neighbourPort = dto.getNeighbourPort();

        result.links.containedBy = oDataIdFromContext(dto.getContainedBy());
        result.links.vlans = oDataIdOfCollectionInContext(dto.getContext(), VLAN);

        return result;
    }

    private static void setStatus(SwitchPortJson json, SwitchPortDto dto) {
        json.status.state = dto.getState();
        json.status.health = dto.getHealth();
        json.status.healthRollup = dto.getHealthRollup();
    }
}
