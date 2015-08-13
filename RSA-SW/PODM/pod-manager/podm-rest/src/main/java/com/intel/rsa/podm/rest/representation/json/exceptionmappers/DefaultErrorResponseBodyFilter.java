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

package com.intel.rsa.podm.rest.representation.json.exceptionmappers;

import com.intel.rsa.podm.rest.representation.json.errors.ErrorResponse;

import javax.ws.rs.container.ContainerRequestContext;
import javax.ws.rs.container.ContainerResponseContext;
import javax.ws.rs.container.ContainerResponseFilter;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.ext.Provider;
import java.io.IOException;

@Provider
public class DefaultErrorResponseBodyFilter implements ContainerResponseFilter {

    @Override
    public void filter(ContainerRequestContext requestContext, ContainerResponseContext responseContext) throws IOException {
        Response.Status.Family family = responseContext.getStatusInfo().getFamily();
        switch (family) {
            case CLIENT_ERROR:
                handleClientError(responseContext);
                break;
            case SERVER_ERROR:
                handleServerError(responseContext);
                break;
            default:
                break;
        }
    }

    private void handleServerError(ContainerResponseContext responseContext) {
        switch ((Response.Status) responseContext.getStatusInfo()) {
            case INTERNAL_SERVER_ERROR:
                setResponseEntityIfRequired(responseContext, ErrorResponse.UNKNOWN_EXCEPTION);
                break;
            default:
                break;
        }
    }

    private void handleClientError(ContainerResponseContext responseContext) {
        switch ((Response.Status) responseContext.getStatusInfo()) {
            case CONFLICT:
                setResponseEntityIfRequired(responseContext, ErrorResponse.RESOURCES_STATE_MISMATCH);
                break;
            case NOT_FOUND:
                setResponseEntityIfRequired(responseContext, ErrorResponse.NOT_FOUND);
                break;
            case METHOD_NOT_ALLOWED:
                setResponseEntityIfRequired(responseContext, ErrorResponse.INVALID_HTTP_METHOD);
                break;
            case NOT_ACCEPTABLE:
                setResponseEntityIfRequired(responseContext, ErrorResponse.BAD_ACCEPT_HEADER);
                break;
            default:
                break;
        }
    }

    private void setResponseEntityIfRequired(ContainerResponseContext responseContext, ErrorResponse error) {
        if (!hasJsonEntity(responseContext)) {
            responseContext.setEntity(error, null, MediaType.APPLICATION_JSON_TYPE);
        }
    }

    private static boolean hasJsonEntity(ContainerResponseContext responseContext) {
        return responseContext.getEntity() != null;
    }
}
