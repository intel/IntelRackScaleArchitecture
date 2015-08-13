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

package com.intel.rsa.client.rss.actions;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.api.actions.RsaApiActionException;
import com.intel.rsa.client.api.actions.rss.RemoteTargetActionInvoker;
import com.intel.rsa.client.webclient.WebClient;
import com.intel.rsa.client.webclient.WebClientImpl;

import java.net.URI;
import java.util.List;

import static java.util.Collections.singletonList;

public class RemoteTargetActionInvokerImpl implements RemoteTargetActionInvoker {
    private WebClient webClient;

    public RemoteTargetActionInvokerImpl(URI baseUri) {
        this.webClient = new WebClientImpl(baseUri);
    }

    @Override
    public void setInitiator(URI targetUri, String initiatorIQN) throws RsaApiActionException {
        webClient.patch(targetUri, new RemoteTargetPatchDto(initiatorIQN));
    }

    private static class RemoteTargetPatchDto {
        @JsonProperty("Initiator")
        List initiators;

        public RemoteTargetPatchDto(String initiator) {
            Object initiatorDto = new Object() {
                @JsonProperty("iSCSI")
                Object x = new Object() {
                    @JsonProperty("InitiatorIQN")
                    String initiatorIqn = initiator;
                };
            };

            this.initiators = singletonList(initiatorDto);
        }
    }
}
