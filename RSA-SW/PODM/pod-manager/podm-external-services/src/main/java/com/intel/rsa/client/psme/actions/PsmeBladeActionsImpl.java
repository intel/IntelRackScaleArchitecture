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

package com.intel.rsa.client.psme.actions;

import com.intel.rsa.client.api.actions.RsaApiActionException;
import com.intel.rsa.client.api.actions.psme.BootSourceOverrideRequest;
import com.intel.rsa.client.api.actions.psme.PsmeBladeActions;
import com.intel.rsa.client.api.actions.psme.ResetRequest;
import com.intel.rsa.client.webclient.WebClient;
import com.intel.rsa.client.webclient.WebClientImpl;

import javax.ws.rs.core.Response;
import java.net.URI;

import static javax.ws.rs.core.Response.Status.Family.SUCCESSFUL;

public class PsmeBladeActionsImpl implements PsmeBladeActions {

    private static final String BLADE_RESET_ACTION_PATH_PART = "/Actions/RSABlade.Reset";

    private WebClient webClient;

    public PsmeBladeActionsImpl(URI baseUri) {
        this(new WebClientImpl(baseUri));
    }

    public PsmeBladeActionsImpl(WebClient webClient) {
        this.webClient = webClient;
    }

    @Override
    public void bootSourceOverride(URI bladeUri, BootSourceOverrideRequest bootSourceOverrideRequest) throws RsaApiActionException {

        Response response = webClient.patch(bladeUri, bootSourceOverrideRequest);

        if (!response.getStatusInfo().getFamily().equals(SUCCESSFUL)) {
            throw new RsaApiActionException("Could not override boot source", bladeUri);
        }
    }

    @Override
    public void reset(URI bladeUri, ResetRequest resetRequest) throws RsaApiActionException {
        URI resetActionUri = URI.create(bladeUri.toString() + BLADE_RESET_ACTION_PATH_PART);

        Response response = webClient.post(resetActionUri, resetRequest);

        if (!response.getStatusInfo().getFamily().equals(SUCCESSFUL)) {
            throw new RsaApiActionException("Could not reset blade", resetActionUri);
        }
    }
}
