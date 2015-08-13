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

import com.intel.rsa.podm.actions.ActionException;
import com.intel.rsa.podm.business.dto.BladeDto;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.BladeBootSourceOverrideService;
import com.intel.rsa.podm.business.services.BladeService;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.rest.error.PodmExceptions;
import com.intel.rsa.podm.rest.representation.json.templates.actions.OverrideBootSourceJson;

import javax.inject.Inject;
import javax.ws.rs.PATCH;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import static com.intel.rsa.podm.rest.resources.ResourceNames.MEMORY_RESOURCE_NAME;
import static com.intel.rsa.podm.rest.resources.ResourceNames.NETWORK_INTERFACES_RESOURCE_NAME;
import static com.intel.rsa.podm.rest.resources.ResourceNames.PROCESSORS_RESOURCE_NAME;
import static com.intel.rsa.podm.rest.resources.ResourceNames.STORAGE_CONTROLLERS_RESOURCE_NAME;
import static javax.ws.rs.core.MediaType.APPLICATION_JSON;

@Produces(APPLICATION_JSON)
public class BladeResource extends BaseResource {
    @Inject
    private BladeService service;

    @Inject
    private BladeBootSourceOverrideService overrideService;

    @Override
    public BladeDto get() {
        Context context = getCurrentContext();
        try {
            return service.getBlade(context);
        } catch (EntityNotFoundException e) {
            throw PodmExceptions.notFound();
        }
    }

    @PATCH
    public void overrideBootSource(OverrideBootSourceJson json) {
        validate(json);

        Context context = getCurrentContext();
        try {
            overrideService.overrideBootSource(context, json.boot.bootSourceType, json.boot.bootSourceState);
        } catch (EntityNotFoundException e) {
            throw PodmExceptions.notFound();
        } catch (ActionException e) {
            throw PodmExceptions.resourcesStateMismatch();
        }
    }

    private void validate(OverrideBootSourceJson json) {
        if (json == null || json.boot == null || json.boot.bootSourceType == null || json.boot.bootSourceState == null) {
            throw PodmExceptions.invalidPayload();
        }
    }

    @Path("Actions")
    public BladeActionsResource getActions() {
        return getResource(BladeActionsResource.class);
    }

    @Path(PROCESSORS_RESOURCE_NAME)
    public ProcessorCollectionResource getProcessors() {
        return getResource(ProcessorCollectionResource.class);
    }

    @Path(MEMORY_RESOURCE_NAME)
    public MemoryCollectionResource getMemory() {
        return getResource(MemoryCollectionResource.class);
    }

    @Path(NETWORK_INTERFACES_RESOURCE_NAME)
    public NetworkInterfaceCollectionResource getEthernetInterfaces() {
        return getResource(NetworkInterfaceCollectionResource.class);
    }

    @Path(STORAGE_CONTROLLERS_RESOURCE_NAME)
    public StorageControllerCollectionResource getStorageControllers() {
        return getResource(StorageControllerCollectionResource.class);
    }
}
