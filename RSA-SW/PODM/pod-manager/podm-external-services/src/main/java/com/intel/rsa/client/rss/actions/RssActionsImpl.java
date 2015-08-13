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

import com.intel.rsa.client.api.actions.RsaApiActionException;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.rss.LogicalDriveCreationRequest;
import com.intel.rsa.client.api.rss.RemoteTargetCreationRequest;
import com.intel.rsa.client.api.rss.RssActions;
import com.intel.rsa.client.rss.RssLogicalDriveImpl;
import com.intel.rsa.client.webclient.WebClientImpl;
import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.common.types.State;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import javax.ws.rs.core.Response;
import java.net.URI;
import java.util.Optional;

import static javax.ws.rs.core.Response.Status.Family.SUCCESSFUL;

@Dependent
public class RssActionsImpl implements RssActions {
    private static final int CHECK_DELAY_MS = 5000;

    @Inject
    private Logger logger;

    @Override
    public URI createLogicalDrive(URI hostUri, URI storageServiceUri, LogicalDriveCreationRequest request)
            throws RsaApiActionException, InterruptedException {
        WebClientImpl client = new WebClientImpl(hostUri);
        URI targetURI = URI.create(storageServiceUri.toString() + "/LogicalDrives");

        logger.i("POSTING TO {} {}", client.getBaseUri(), targetURI);
        Response response = client.post(targetURI, request);

        //todo consider moving this to WebClient
        if (!response.getStatusInfo().getFamily().equals(SUCCESSFUL)) {
            throw new RsaApiActionException("Could not create logical drive", targetURI);
        }

        URI location = response.getLocation();

        return waitForEnabledState(hostUri, location);
    }

    private URI waitForEnabledState(URI baseUri, URI location) throws RsaApiActionException, InterruptedException {
        URI path = URI.create(location.getPath());

        while (true) {
            Optional<State> driveState = readDriveState(baseUri, path);

            if (driveState.isPresent()) {
                switch (driveState.get()) {
                    case ENABLED:
                        return location;
                    case ABSENT:
                        throw new RsaApiActionException("Error occurred during creation of logical drive", location);
                    default:
                        break;
                }
            }

            Thread.sleep(CHECK_DELAY_MS);
        }
    }

    private Optional<State> readDriveState(URI baseUri, URI path) {
        try (WebClientImpl webClient = new WebClientImpl(baseUri)) {
            RssLogicalDriveImpl drive = webClient.get(path, RssLogicalDriveImpl.class);
            return Optional.ofNullable(drive.getState());
        } catch (RsaApiReaderException e) {
            logger.d("Could not read logical drive", e);
            return Optional.empty();
        }
    }

    @Override
    public URI createRemoteTarget(URI hostUri, URI storageServiceUri, RemoteTargetCreationRequest request) throws RsaApiActionException {
        WebClientImpl client = new WebClientImpl(hostUri);
        URI targetURI = URI.create(storageServiceUri.toString() + "/Targets");

        logger.i("POSTING TO {} {}", client.getBaseUri(), targetURI);
        Response response = client.post(targetURI, request);

        //todo consider moving this to WebClient
        if (!response.getStatusInfo().getFamily().equals(SUCCESSFUL)) {
            throw new RsaApiActionException("Could not create target", targetURI);
        }

        return response.getLocation();
    }
}
