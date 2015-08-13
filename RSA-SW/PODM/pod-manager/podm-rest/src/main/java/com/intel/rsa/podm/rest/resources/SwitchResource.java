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

import com.intel.rsa.podm.business.dto.SwitchDto;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.SwitchService;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.rest.error.PodmExceptions;

import javax.inject.Inject;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import static com.intel.rsa.podm.rest.resources.ResourceNames.SWITCH_PORTS_RESOURCE_NAME;
import static javax.ws.rs.core.MediaType.APPLICATION_JSON;

@Produces(APPLICATION_JSON)
public class SwitchResource extends BaseResource {

    @Inject
    private SwitchService switchService;

    @Override
    public SwitchDto get() {
        Context context = getCurrentContext();
        try {
            return switchService.getSwitch(context);
        } catch (EntityNotFoundException e) {
            throw PodmExceptions.notFound();
        }
    }

    @Path(SWITCH_PORTS_RESOURCE_NAME)
    public SwitchPortCollectionResource getSwitchPorts() {
        return getResource(SwitchPortCollectionResource.class);
    }
}
