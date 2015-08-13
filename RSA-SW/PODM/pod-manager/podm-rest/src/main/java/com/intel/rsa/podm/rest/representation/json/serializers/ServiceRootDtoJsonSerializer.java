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

import com.intel.rsa.podm.business.dto.ServiceRootDto;
import com.intel.rsa.podm.rest.representation.json.templates.ServiceRootJson;

import static com.intel.rsa.podm.business.services.context.ContextType.MANAGER;
import static com.intel.rsa.podm.business.services.context.ContextType.POD;
import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfCollectionInContext;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfSystems;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfServices;
import static java.net.URI.create;

public final class ServiceRootDtoJsonSerializer extends DtoJsonSerializer<ServiceRootDto> {
    public ServiceRootDtoJsonSerializer() {
        super(ServiceRootDto.class);
    }

    @Override
    protected ServiceRootJson translate(ServiceRootDto dto) {
        ServiceRootJson result = new ServiceRootJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAPODMServiceRoot");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.modified = dto.getModified();

        result.time = dto.getTime();
        result.serviceVersion = dto.getServiceVersion();
        result.uuid = dto.getUuid();
        result.name = dto.getName();

        result.links.chassis = oDataIdOfCollectionInContext(null, POD);
        result.links.services = oDataIdOfServices();
        result.links.systems = oDataIdOfSystems();
        result.links.managers = oDataIdOfCollectionInContext(null, MANAGER);

        return result;
    }
}
