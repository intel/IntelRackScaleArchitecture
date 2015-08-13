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

import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.rest.representation.json.templates.CollectionJson;

import java.util.stream.Collectors;

import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;

public class CollectionDtoJsonSerializer extends DtoJsonSerializer<CollectionDto> {
    public CollectionDtoJsonSerializer() {
        super(CollectionDto.class);
    }

    @Override
    protected CollectionJson translate(CollectionDto dto) {
        ODataForCollection oData = CollectionTypeToCollectionODataMapping.getOdataForCollectionType(dto.getType());

        CollectionJson result = new CollectionJson(oData.getODataType());
        result.name = oData.getName();
        result.oDataContext = oData.getODataContext();
        result.oDataId = oDataId(context.getRequestPath());
        result.modified = dto.getModified();

        result.links.members.addAll(dto.getMembers().stream()
                .map(id -> oDataId(result.oDataId, id))
                .collect(Collectors.toList()));

        return result;
    }
}
