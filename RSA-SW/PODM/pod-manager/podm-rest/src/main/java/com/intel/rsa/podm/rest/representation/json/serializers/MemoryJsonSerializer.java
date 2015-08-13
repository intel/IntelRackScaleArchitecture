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

import com.intel.rsa.podm.business.dto.MemoryDto;
import com.intel.rsa.podm.rest.representation.json.templates.MemoryJson;

import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdFromContext;
import static java.net.URI.create;

public class MemoryJsonSerializer extends DtoJsonSerializer<MemoryDto> {
    public MemoryJsonSerializer() {
        super(MemoryDto.class);
    }

    @Override
    protected MemoryJson translate(MemoryDto dto) {
        MemoryJson result = new MemoryJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAMemory/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.location = toJson(dto.getLocation());
        result.modified = dto.getModified();

        result.manufacturer = dto.getManufacturer();
        result.socket = dto.getSocket();
        result.bank = dto.getBank();
        result.type = dto.getType();
        result.sizeGB = dto.getSizeGB();
        result.speedMHz = dto.getSpeedMHz();
        result.voltageVolt = dto.getVoltageVolt();
        result.dataWidthBits = dto.getDataWidthBits();
        result.totalWidthBits = dto.getTotalWidthBits();
        result.formFactor = dto.getFormFactor();
        result.serialNumber = dto.getSerialNumber();
        result.assetTag = dto.getAssetTag();
        result.partNumber = dto.getPartNumber();
        result.rank = dto.getRank();
        result.configuredSpeedMHz = dto.getConfiguredSpeedMHz();
        result.minimumVoltageVolt = dto.getMinimumVoltageVolt();
        result.maximumVoltageVolt = dto.getMaximumVoltageVolt();
        result.serialNumber = dto.getSerialNumber();
        mapStatus(dto, result);

        result.links.containedBy = oDataIdFromContext(dto.getContext().getParent());

        return result;
    }

    private void mapStatus(MemoryDto dto, MemoryJson result) {
        result.status.state = dto.getState();
        result.status.health = dto.getHealth();
    }

}
