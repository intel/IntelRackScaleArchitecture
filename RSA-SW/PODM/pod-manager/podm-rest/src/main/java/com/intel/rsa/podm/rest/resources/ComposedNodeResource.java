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

import com.intel.rsa.podm.business.dto.ComposedNodeDto;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.exceptions.EntityOperationException;
import com.intel.rsa.podm.business.services.ComposedNodeService;
import com.intel.rsa.podm.rest.error.PodmExceptions;

import javax.inject.Inject;
import javax.ws.rs.DELETE;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Response;

import static javax.ws.rs.core.MediaType.APPLICATION_JSON;

/**
 * Member of composed node global collection.
 */
@Produces(APPLICATION_JSON)
public class ComposedNodeResource extends BaseResource {

    @Inject
    private ComposedNodeService service;

    @Override
    public ComposedNodeDto get() {
        try {
            return service.getComposedNode(getCurrentContext());
        } catch (EntityNotFoundException e) {
            throw PodmExceptions.notFound();
        }
    }

    @DELETE
    @Override
    public Response delete() {
        try {
            service.deleteComposedNode(getCurrentContext());
        } catch (EntityNotFoundException e) {
            throw PodmExceptions.notFound();
        } catch (EntityOperationException e) {
            throw new RuntimeException("The specified node could not be disassembled");
        }
        return Response.noContent().build();
    }

    @Path("Actions")
    public ComposedNodeActionsResource getActions() {
        return getResource(ComposedNodeActionsResource.class);
    }
}
