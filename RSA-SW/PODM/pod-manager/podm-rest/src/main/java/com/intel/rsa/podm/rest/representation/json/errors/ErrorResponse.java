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

package com.intel.rsa.podm.rest.representation.json.errors;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonValue;
import com.intel.rsa.podm.rest.representation.json.templates.httpresponse.StatusResponseJson;

import javax.ws.rs.core.Response;

import static javax.ws.rs.core.Response.Status.BAD_REQUEST;
import static javax.ws.rs.core.Response.Status.CONFLICT;
import static javax.ws.rs.core.Response.Status.INTERNAL_SERVER_ERROR;
import static javax.ws.rs.core.Response.Status.METHOD_NOT_ALLOWED;
import static javax.ws.rs.core.Response.Status.NOT_ACCEPTABLE;
import static javax.ws.rs.core.Response.Status.REQUEST_URI_TOO_LONG;

public enum ErrorResponse {
    UNKNOWN_EXCEPTION(INTERNAL_SERVER_ERROR, "UnknownException",
            "The server encountered an unexpected condition that prevented it from fulfilling the request"),
    NOT_FOUND(Response.Status.NOT_FOUND, "InvalidEndpoint", "Invalid endpoint in /rest/v1 namespace"),
    RESOURCES_STATE_MISMATCH(CONFLICT, "ResourcesStateMismatch", "Conflict during allocation"),
    INVALID_HTTP_METHOD(METHOD_NOT_ALLOWED, "InvalidHttpMethod", "Invalid HTTP request method"),
    MALFORMED_URI(BAD_REQUEST, "MalformedUri", "Malformed URI"),
    BAD_ACCEPT_HEADER(NOT_ACCEPTABLE, "BadAcceptHeader", "Bad Accept field in request header"),
    URI_TOO_LONG(REQUEST_URI_TOO_LONG, "RequestUriTooLong", "Malformed URI"),
    INVALID_PAYLOAD(BAD_REQUEST, "InvalidPayload", "Request payload is invalid or missing");

    private static final String ERROR_CODE_PREFIX = "Base.1.0.";

    @JsonIgnore
    private final int errorCode;

    @JsonProperty
    private final Object error;

    ErrorResponse(Response.Status status, String errorType, String errorMessage) {
        this.errorCode = status.getStatusCode();
        this.error = new Object() {
            @JsonProperty("error")
            Object error = new StatusResponseJson(ERROR_CODE_PREFIX + errorType, errorMessage);
        };
    }

    public int getErrorCode() {
        return errorCode;
    }

    @JsonValue
    public Object toJson() {
        return error;
    }
}
