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
import com.intel.rsa.client.api.rss.IScsiAddress;
import com.intel.rsa.client.api.rss.IScsiAddressWrapper;
import com.intel.rsa.client.api.rss.IScsiInitiatorWrapper;
import com.intel.rsa.client.api.rss.InitiatorIQN;
import com.intel.rsa.client.api.rss.LogicalDriveCreationRequest;
import com.intel.rsa.client.api.rss.ODataIdWrapper;
import com.intel.rsa.client.api.rss.RemoteTargetCreationRequest;
import com.intel.rsa.client.api.rss.RequestFactory;
import com.intel.rsa.client.api.rss.TargetLUN;

import javax.enterprise.context.Dependent;
import java.net.URI;

@Dependent
public class RequestFactoryImpl implements RequestFactory {

    @Override
    public LogicalDriveCreationRequest getLogicalDriveCreationRequest() {
        return new LogicalDriveCreationRequestImpl();
    }

    @Override
    public RemoteTargetCreationRequest getRemoteTargetCreationRequest() {
        return new RemoteTargetCreationRequestImpl();
    }

    @Override
    public ODataIdWrapper getODataIdWrapper(URI odataUri) {
        return new ODataIdWrapperImpl(odataUri);
    }

    @Override
    public IScsiAddress getIScsiAddress() {
        return new IScsiAddressImpl();
    }

    @Override
    public TargetLUN getTargetLUN() {
        return new TargetLUNImpl();
    }

    @Override
    public IScsiInitiatorWrapper getIscsiInitiator() {
        return new IScsiInitiatorWrapperImpl();
    }

    @Override
    public InitiatorIQN getInititatorIQN() {
        return new InitiatorIQNImpl();
    }

    @Override
    public IScsiAddressWrapper getIScsiAddressWrapper(IScsiAddress iScsiAddress) {
        return new IScsiAddressWrapper() {

            @Override
            @JsonProperty("iSCSI")
            public IScsiAddress getAddress() {
                return iScsiAddress;
            }
        };
    }
}
