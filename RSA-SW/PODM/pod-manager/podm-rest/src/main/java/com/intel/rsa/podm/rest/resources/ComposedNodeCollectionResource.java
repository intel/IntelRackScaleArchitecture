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

import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.dto.RequestedRemoteDrive;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.exceptions.EntityOperationException;
import com.intel.rsa.podm.business.services.ComposedNodeService;
import com.intel.rsa.podm.rest.error.PodmExceptions;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.assembly.AllocationTemplate;
import com.intel.rsa.podm.rest.representation.json.templates.assembly.RequestedNodeDefaultImpl;
import com.intel.rsa.podm.rest.validators.RequestedRemoteDrivesValidator;

import javax.inject.Inject;
import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Response;
import java.net.URI;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static javax.ws.rs.core.MediaType.APPLICATION_JSON;

/**
 * Composed Nodes global collection.
 */
@Produces(APPLICATION_JSON)
public class ComposedNodeCollectionResource extends BaseResource {

    @Inject
    private ComposedNodeService service;

    @Inject
    private RequestedRemoteDrivesValidator remoteDrivesValidator;

    @Override
    public CollectionDto get() {
        try {
            return service.getCollection();
        } catch (EntityNotFoundException e) {
            throw PodmExceptions.notFound();
        }
    }

    @Path("{nodeId}")
    public ComposedNodeResource getNode() {
        return getResource(ComposedNodeResource.class);
    }

    private Response createComposedNode(RequestedNode requestedNode) {
        try {
            ODataId oDataId = ODataIds.oDataIdFromContext(service.createComposedNode(requestedNode));
            return Response.created(URI.create(oDataId.toString())).build();
        } catch (EntityOperationException e) {
            throw PodmExceptions.resourcesStateMismatch();
        }
    }

    @POST
    @Consumes(APPLICATION_JSON)
    public Response postJson(AllocationTemplate allocationTemplate) {
        RequestedNode requestedNode;
        if (allocationTemplate.getSystems().isEmpty()) {
            requestedNode = new RequestedNodeDefaultImpl();
        } else if (allocationTemplate.getSystems().size() == 1) {
            requestedNode = allocationTemplate.getSystems().get(0);
        } else {
            throw PodmExceptions.invalidPayload();
        }

        if (containsRemoteDrives(requestedNode)) {

            if (requestedNode.getRemoteDrives().size() != 1) {
                throw PodmExceptions.invalidPayload(); // only 1 remote drive is valid for now
            }

            RequestedRemoteDrive drive = single(requestedNode.getRemoteDrives());

            if (!remoteDrivesValidator.driveIsValid(drive)) {
                throw PodmExceptions.invalidPayload();
            }
        }

        return createComposedNode(requestedNode);
    }

    private boolean containsRemoteDrives(RequestedNode requestedNode) {
        return requestedNode.getRemoteDrives() != null && !requestedNode.getRemoteDrives().isEmpty();
    }
}
