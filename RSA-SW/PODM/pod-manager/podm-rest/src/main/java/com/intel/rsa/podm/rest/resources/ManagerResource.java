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

package com.intel.rsa.podm.rest.resources;

import com.intel.rsa.podm.business.dto.ManagerDto;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.ManagerService;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.rest.error.PodmExceptions;

import javax.inject.Inject;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import static com.intel.rsa.podm.rest.resources.ResourceNames.NETWORK_INTERFACES_RESOURCE_NAME;
import static com.intel.rsa.podm.rest.resources.ResourceNames.NETWORK_SERVICE_RESOURCE_NAME;
import static javax.ws.rs.core.MediaType.APPLICATION_JSON;

/**
 * Member of managers global collection.
 */
@Produces(APPLICATION_JSON)
public class ManagerResource extends BaseResource {

    @Inject
    private ManagerService service;

    @Override
    public ManagerDto get() {
        try {
            Context context = getCurrentContext();
            return service.getManager(context);
        } catch (EntityNotFoundException e) {
            throw PodmExceptions.notFound();
        }
    }

    @Path(NETWORK_SERVICE_RESOURCE_NAME)
    public NetworkServiceResource getNetworkService() {
        return getResource(NetworkServiceResource.class);
    }

    @Path(NETWORK_INTERFACES_RESOURCE_NAME)
    public NetworkInterfaceCollectionResource getNetworkInterfaces() {
        return getResource(NetworkInterfaceCollectionResource.class);
    }
}
