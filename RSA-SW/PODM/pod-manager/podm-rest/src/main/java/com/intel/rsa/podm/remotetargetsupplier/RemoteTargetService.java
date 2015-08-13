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
package com.intel.rsa.podm.remotetargetsupplier;

import javax.inject.Inject;
import javax.ws.rs.GET;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;

import static javax.ws.rs.core.MediaType.TEXT_PLAIN;

@Path("iscsitarget")
public class RemoteTargetService {
    @Inject
    private RemoteTargetDirectory directory;

    @GET
    @Produces(TEXT_PLAIN)
    public Object getRemoteTarget(@QueryParam("mac") String macAddress) {
        try {
            return directory.getRemoteTarget(macAddress).toString();
        } catch (RemoteTargetNotFoundException e) {
            throw new NotFoundException();
        }
    }
}
