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

import com.intel.rsa.podm.business.dto.ServiceRootDto;
import com.intel.rsa.podm.business.services.ServiceRootService;

import javax.inject.Inject;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import static com.intel.rsa.podm.rest.resources.ResourceNames.COMPOSED_NODES_RESOURCE_NAME;
import static com.intel.rsa.podm.rest.resources.ResourceNames.MANAGERS_RESOURCE_NAME;
import static com.intel.rsa.podm.rest.resources.ResourceNames.PODS_RESOURCE_NAME;
import static com.intel.rsa.podm.rest.resources.ResourceNames.REMOTE_TARGETS_RESOURCE_NAME;
import static com.intel.rsa.podm.rest.resources.ResourceNames.STORAGE_SERVICES_RESOURCE_NAME;
import static javax.ws.rs.core.MediaType.APPLICATION_JSON;

@Path("/rest/v1")
@Produces(APPLICATION_JSON)
public class ServiceRootResource extends BaseResource {
    @Inject
    private ServiceRootService service;

    @Override
    public ServiceRootDto get() {
        return service.getServiceRoot();
    }

    @Path(PODS_RESOURCE_NAME)
    public PodCollectionResource getPods() {
        return getResource(PodCollectionResource.class);
    }

    @Path(COMPOSED_NODES_RESOURCE_NAME)
    public ComposedNodeCollectionResource getNodes() {
        return getResource(ComposedNodeCollectionResource.class);
    }

    @Path(MANAGERS_RESOURCE_NAME)
    public ManagerCollectionResource getManagers() {
        return getResource(ManagerCollectionResource.class);
    }

    @Path(STORAGE_SERVICES_RESOURCE_NAME)
    public StorageServiceCollectionResource getStorageServices() {
        return getResource(StorageServiceCollectionResource.class);
    }

    @Path(REMOTE_TARGETS_RESOURCE_NAME)
    public RemoteTargetCollectionResource getRemoteTargets() {
        return getResource(RemoteTargetCollectionResource.class);
    }
}
