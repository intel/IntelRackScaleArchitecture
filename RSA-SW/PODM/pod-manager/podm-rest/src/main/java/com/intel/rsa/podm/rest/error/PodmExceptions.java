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

package com.intel.rsa.podm.rest.error;

import com.intel.rsa.podm.rest.representation.json.errors.ErrorResponse;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import static javax.ws.rs.core.MediaType.APPLICATION_JSON;

public final class PodmExceptions {
    private PodmExceptions() {
    }

    public static WebApplicationException invalidPayload() {
        return createException(ErrorResponse.INVALID_PAYLOAD);
    }

    public static WebApplicationException notFound() {
        return createException(ErrorResponse.NOT_FOUND);
    }

    public static WebApplicationException resourcesStateMismatch() {
        return createException(ErrorResponse.RESOURCES_STATE_MISMATCH);
    }

    private static WebApplicationException createException(ErrorResponse errorResponse) {
        Response response = toJaxRsResponse(errorResponse);
        return new WebApplicationException(response);
    }

    private static Response toJaxRsResponse(ErrorResponse error) {
        return Response
                .status(error.getErrorCode())
                .type(APPLICATION_JSON)
                .entity(error)
                .build();
    }
}
