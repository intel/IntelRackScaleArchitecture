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

import com.intel.rsa.podm.business.dto.SwitchDto;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.rest.representation.json.templates.SwitchJson;

import static com.intel.rsa.podm.business.services.context.ContextType.SWITCH_PORT;
import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdFromContext;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfCollectionInContext;
import static java.net.URI.create;

public class SwitchDtoJsonSerializer extends DtoJsonSerializer<SwitchDto> {
    protected SwitchDtoJsonSerializer() {
        super(SwitchDto.class);
    }

    @Override
    protected SwitchJson translate(SwitchDto dto) {
        SwitchJson result = new SwitchJson();

        result.oDataContext = create("/rest/v1/$metadata#RSASwitches/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.location = toJson(dto.getLocation());
        result.modified = dto.getModified();

        result.status.state = dto.getState();
        result.status.health = dto.getHealth();
        result.status.healthRollup = dto.getHealthRollup();

        result.links.switchPorts = oDataIdOfCollectionInContext(dto.getContext(), SWITCH_PORT);
        result.links.containedBy = oDataIdFromContext(dto.getContainedBy());

        for (Context managerContext: dto.getManagedBy()) {
            result.links.managedBy.add(oDataIdFromContext(managerContext));
        }

        return result;
    }
}
