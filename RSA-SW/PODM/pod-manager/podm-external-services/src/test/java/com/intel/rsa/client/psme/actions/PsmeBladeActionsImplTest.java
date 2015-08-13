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
import com.intel.rsa.client.api.actions.psme.ResetRequestFactory;
import com.intel.rsa.client.webclient.WebClient;
import com.intel.rsa.common.types.BootSourceState;
import com.intel.rsa.common.types.BootSourceType;
import com.intel.rsa.common.types.actions.ResetType;
import org.testng.annotations.Test;

import javax.ws.rs.core.Response;
import java.net.URI;

import static org.mockito.Matchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class PsmeBladeActionsImplTest {
    private static final URI ANY_URI = URI.create("");

    @Test(expectedExceptions = RsaApiActionException.class)
    public void whenResetWithErrorResponse_ShouldThrowException() throws RsaApiActionException {
        ResetType anyResetType = ResetType.ON;
        ResetRequestFactory resetRequestFactory = new ResetRequestFactoryImpl();
        ResetRequest resetRequest = resetRequestFactory.create(anyResetType);

        PsmeBladeActions psmeBladeActions = createPsmeBladeActions(Response.serverError().build());
        psmeBladeActions.reset(ANY_URI, resetRequest);
    }

    @Test
    public void whenResetWithOkResponse_ShouldNotThrowException() throws RsaApiActionException {
        ResetType anyResetType = ResetType.ON;
        PsmeBladeActions psmeBladeActions = createPsmeBladeActions(Response.noContent().build());
        ResetRequestFactory resetRequestFactory = new ResetRequestFactoryImpl();
        ResetRequest resetRequest = resetRequestFactory.create(anyResetType);

        psmeBladeActions.reset(ANY_URI, resetRequest);
    }

    @Test(expectedExceptions = RsaApiActionException.class)
    public void whenBootSourceOverrideWithErrorResponse_ShouldThrowException() throws RsaApiActionException {
        BootSourceType anyBootSourceType = BootSourceType.HDD;
        BootSourceState anyBootSourceState = BootSourceState.CONTINUOUS;

        PsmeBladeActions psmeBladeActions = createPsmeBladeActions(Response.serverError().build());
        BootSourceOverrideRequestFactoryImpl bootSourceOverrideRequestFactory = new BootSourceOverrideRequestFactoryImpl();

        BootSourceOverrideRequest bootSourceOverrideRequest = bootSourceOverrideRequestFactory.create(anyBootSourceType, anyBootSourceState);
        psmeBladeActions.bootSourceOverride(ANY_URI, bootSourceOverrideRequest);
    }

    @Test
    public void whenBootSourceOverrideWithOkResponse_ShouldNotThrowException() throws RsaApiActionException {
        BootSourceType anyBootSourceType = BootSourceType.HDD;
        BootSourceState anyBootSourceState = BootSourceState.CONTINUOUS;

        PsmeBladeActions psmeBladeActions = createPsmeBladeActions(Response.ok().build());
        BootSourceOverrideRequestFactoryImpl bootSourceOverrideRequestFactory = new BootSourceOverrideRequestFactoryImpl();

        BootSourceOverrideRequest bootSourceOverrideRequest = bootSourceOverrideRequestFactory.create(anyBootSourceType, anyBootSourceState);
        psmeBladeActions.bootSourceOverride(ANY_URI, bootSourceOverrideRequest);
    }

    private WebClient createWebClient(Response response) throws RsaApiActionException {
        WebClient webClientMock = mock(WebClient.class);
        when(webClientMock.post(any(URI.class), any(Object.class))).thenReturn(response);
        when(webClientMock.patch(any(URI.class), any(Object.class))).thenReturn(response);

        return webClientMock;
    }

    private PsmeBladeActions createPsmeBladeActions(Response response) throws RsaApiActionException {
        return new PsmeBladeActionsImpl(createWebClient(response));
    }
}
