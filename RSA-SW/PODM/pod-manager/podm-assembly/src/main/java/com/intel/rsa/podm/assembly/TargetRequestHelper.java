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

package com.intel.rsa.podm.assembly;

import com.intel.rsa.client.api.rss.IScsiAddress;
import com.intel.rsa.client.api.rss.IScsiAddressWrapper;
import com.intel.rsa.client.api.rss.IScsiInitiatorWrapper;
import com.intel.rsa.client.api.rss.InitiatorIQN;
import com.intel.rsa.client.api.rss.LogicalDriveLinks;
import com.intel.rsa.client.api.rss.ODataIdWrapper;
import com.intel.rsa.client.api.rss.RemoteTargetCreationRequest;
import com.intel.rsa.client.api.rss.RequestFactory;
import com.intel.rsa.client.api.rss.TargetLUN;
import com.intel.rsa.podm.business.dto.RequestedRemoteDrive;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.net.URI;
import java.util.List;

@Dependent
public class TargetRequestHelper {

    @Inject
    private RequestFactory requestFactory;

    public void fillInLinks(URI newLogicalDriveURI, RemoteTargetCreationRequest request) {
        LogicalDriveLinks logicalDriveLinks = request.getLogicalDriveLinks();

        ODataIdWrapper oDataIdWrapper = requestFactory.getODataIdWrapper(newLogicalDriveURI);

        logicalDriveLinks.getODataIdWrappers().add(oDataIdWrapper);
    }

    public void fillInIScsiInitiators(RemoteTargetCreationRequest request) {
        List<IScsiInitiatorWrapper> iScsiInitiatorWrappers = request.getIScsiInitiators();

        IScsiInitiatorWrapper iScsiInitiatorWrapper = requestFactory.getIscsiInitiator();
        InitiatorIQN initiatorIQN = requestFactory.getInititatorIQN();
        initiatorIQN.setIQN("ALL");
        iScsiInitiatorWrapper.setInitiatorIQN(initiatorIQN);
        iScsiInitiatorWrappers.add(iScsiInitiatorWrapper);
    }

    public void fillIScsiAddresses(RequestedRemoteDrive template, URI newLogicalDriveURI, RemoteTargetCreationRequest request) {
        IScsiAddress iScsiAddress = requestFactory.getIScsiAddress();

        List<TargetLUN> targetLUNs = iScsiAddress.getTargetLUNs();

        TargetLUN lun = requestFactory.getTargetLUN();

        lun.setLUN(1);
        lun.setDriveURI(newLogicalDriveURI);

        targetLUNs.add(lun);

        iScsiAddress.setTargetIQN(template.getAddress());

        IScsiAddressWrapper wrapper = requestFactory.getIScsiAddressWrapper(iScsiAddress);

        List<IScsiAddressWrapper> iScsiAddresses = request.getIScsiAddressWrappers();
        iScsiAddresses.add(wrapper);
    }
}
