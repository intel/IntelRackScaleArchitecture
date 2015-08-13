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


import javax.ws.rs.GET;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Response;
import java.io.InputStream;


public class StreamResource {
    private final String resourceName;
    private final String mediaType;

    public StreamResource(String resourceName, String mediaType) {
        this.resourceName = resourceName;
        this.mediaType = mediaType;
    }

    @GET
    @Produces
    public Response get() {
        InputStream inputStream = this.getClass()
                .getClassLoader()
                .getResourceAsStream(resourceName);

        return Response
                .ok(inputStream)
                .type(mediaType)
                .build();
    }

}
