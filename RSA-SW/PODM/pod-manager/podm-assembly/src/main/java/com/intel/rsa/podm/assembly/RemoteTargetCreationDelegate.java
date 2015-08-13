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

import com.intel.rsa.client.api.actions.RsaApiActionException;
import com.intel.rsa.client.api.rss.LogicalDriveCreationRequest;
import com.intel.rsa.client.api.rss.ODataIdWrapper;
import com.intel.rsa.client.api.rss.RemoteTargetCreationRequest;
import com.intel.rsa.client.api.rss.RequestFactory;
import com.intel.rsa.client.api.rss.RssActions;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.ReplicationMethod;
import com.intel.rsa.podm.business.dto.RequestedRemoteDrive;
import com.intel.rsa.podm.business.entities.assets.LogicalDrive;
import com.intel.rsa.podm.business.entities.components.ComposedNode;

import javax.annotation.Resource;
import javax.enterprise.concurrent.ManagedExecutorService;
import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.net.URI;
import java.util.concurrent.Future;

import static com.intel.rsa.common.types.ComposedNodeState.FAILED;

@Dependent
public class RemoteTargetCreationDelegate {

    @Inject
    RemoteTargetCreationHandler handler;

    @Inject
    private RssActions rssActions;

    @Inject
    private RequestFactory requestFactory;

    // TODO: This needs clean-up?
    @Inject
    private TargetRequestHelper requestHelper;

    @Resource
    private ManagedExecutorService executorService;

    public void createRemoteTarget(ComposedNode node, LogicalDrive lvg, URI masterDriveSourceUri, RequestedRemoteDrive template) throws AllocationException {
        URI hostUri = lvg.getStorageService().getService().getBaseUri();
        URI storageServiceUri = lvg.getStorageService().getSourceUri();
        URI lvgSourceUri = lvg.getSourceUri();
        Id nodeId = node.getId();

        Future<URI> uri = executorService.submit(() -> {
                try {
                    URI newLogicalDriveUri = sendLogicalDriveCreationRequest(hostUri, storageServiceUri, lvgSourceUri, masterDriveSourceUri, template);
                    return sendRemoteTargetCreationRequest(hostUri, storageServiceUri, template, newLogicalDriveUri);
                } catch (AllocationException | InterruptedException e) {
                    node.setComposedNodeState(FAILED);
                    throw new RuntimeException(e);
                }
            }
        );
        handler.registerDelegatedRequest(nodeId, uri);
    }

    private URI sendLogicalDriveCreationRequest(URI hostUri, URI storageServiceUri, URI lvgSourceUri,
                URI masterDriveSourceUri, RequestedRemoteDrive template)
            throws AllocationException, InterruptedException {
        LogicalDriveCreationRequest request = buildLogicalDriveRequest(template, lvgSourceUri, masterDriveSourceUri);

        try {
            return rssActions.createLogicalDrive(hostUri, storageServiceUri, request);
        } catch (RsaApiActionException e) {
            throw new AllocationException("Logical Drive Creation Failed", e);
        }
    }

    private URI sendRemoteTargetCreationRequest(URI hostUri, URI storageServiceUri, RequestedRemoteDrive template, URI newLogicalDriveURI)
            throws AllocationException {

        URI uri = URI.create(newLogicalDriveURI.getPath());
        RemoteTargetCreationRequest request = buildRemoteTargetRequest(template, uri);

        try {
            return rssActions.createRemoteTarget(hostUri, storageServiceUri, request);
        } catch (RsaApiActionException e) {
            throw new AllocationException("Logical Drive Creation Failed", e);
        }
    }

    private LogicalDriveCreationRequest buildLogicalDriveRequest(RequestedRemoteDrive template, URI lvgSourceUri, URI masterDriveSourceUri) {
        LogicalDriveCreationRequest request = requestFactory.getLogicalDriveCreationRequest();
        request.setSnapshot(template.getMaster().getType() == ReplicationMethod.SNAPSHOT);
        request.setCapacityGB(template.getCapacityGB());

        ODataIdWrapper masterDriveODataId = requestFactory.getODataIdWrapper(masterDriveSourceUri);
        request.getLinks().setMasterDrive(masterDriveODataId);

        ODataIdWrapper lvgOdataId = requestFactory.getODataIdWrapper(lvgSourceUri);
        request.getLinks().getLogicalDrivesList().add(lvgOdataId);

        return request;
    }

    private RemoteTargetCreationRequest buildRemoteTargetRequest(RequestedRemoteDrive template, URI newLogicalDriveURI) {
        RemoteTargetCreationRequest request = requestFactory.getRemoteTargetCreationRequest();

        requestHelper.fillIScsiAddresses(template, newLogicalDriveURI, request);
        requestHelper.fillInIScsiInitiators(request);
        requestHelper.fillInLinks(newLogicalDriveURI, request);

        return request;
    }
}
