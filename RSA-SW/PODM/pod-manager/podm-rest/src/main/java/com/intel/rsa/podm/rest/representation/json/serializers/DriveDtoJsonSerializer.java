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

import com.intel.rsa.podm.business.dto.DriveDto;
import com.intel.rsa.podm.rest.representation.json.templates.DriveJson;

import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdFromContext;
import static java.net.URI.create;

public class DriveDtoJsonSerializer extends DtoJsonSerializer<DriveDto> {
    public DriveDtoJsonSerializer() {
        super(DriveDto.class);
    }

    @Override
    protected DriveJson translate(DriveDto dto) {
        DriveJson result = new DriveJson();

        result.oDataContext = create("/rest/v1/$metadata#Drive/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.modified = dto.getModified();
        result.controllerInterface = dto.getControllerInterface();
        result.capacityGB = dto.getCapacityGB();
        result.manufacturer = dto.getManufacturer();
        result.model = dto.getModel();
        result.rpm = dto.getRpm();
        result.serialNumber = dto.getSerialNumber();
        result.location = toJson(dto.getLocation());
        result.type = dto.getType();
        mapStatus(dto, result);

        result.links.containedBy = oDataIdFromContext(dto.getContext().getParent());

        return result;
    }

    private void mapStatus(DriveDto dto, DriveJson result) {
        result.status.state = dto.getState();
        result.status.health = dto.getHealth();
        result.status.healthRollup = dto.getHealthRollup();
    }

}
