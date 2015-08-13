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
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.BladeResetService;
import com.intel.rsa.podm.rest.error.PodmExceptions;
import com.intel.rsa.podm.rest.representation.json.templates.actions.ResetTypeJson;

import javax.inject.Inject;
import javax.ws.rs.ClientErrorException;
import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Response;

import static javax.ws.rs.core.MediaType.APPLICATION_JSON;
import static javax.ws.rs.core.Response.Status.METHOD_NOT_ALLOWED;

@Produces(APPLICATION_JSON)
public class BladeActionsResource extends BaseResource {

    @Inject
    private BladeResetService service;


    @Override
    public Object get() {
        // Single actions are only available, no collection is provided
        throw PodmExceptions.notFound();
    }

    @POST
    @Consumes(APPLICATION_JSON)
    @Path("RSABlade.Reset")
    public Response reset(ResetTypeJson resetTypeJson) {
        if (resetTypeJson.resetType == null) {
            throw PodmExceptions.invalidPayload();
        }

        try {
            service.reset(getCurrentContext(), resetTypeJson.resetType);
        } catch (EntityNotFoundException e) {
            throw PodmExceptions.notFound();
        } catch (ActionException e) {
            throw new ClientErrorException("Reset action could not be completed!", METHOD_NOT_ALLOWED);
        }
        return Response.noContent().build();
    }
}
