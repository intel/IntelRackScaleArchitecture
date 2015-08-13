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

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.business.entities.components.ComposedNode;
import com.intel.rsa.podm.business.entities.events.RemoteTargetCreated;

import javax.ejb.Schedule;
import javax.ejb.Singleton;
import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;
import javax.enterprise.event.Observes;
import javax.enterprise.event.TransactionPhase;
import javax.inject.Inject;
import java.net.URI;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

@Singleton
public class RemoteTargetCreationHandler {
    private List<DelegatedRemoteTargetCreation> delegatedRemoteTargetCreations = new ArrayList<>();

    @Inject
    private DomainObjectRepository repository;

    @Inject
    private NodeComposer nodeComposer;

    @Inject
    private Logger logger;

    @Schedule(hour = "*", minute = "*", second = "*/30")
    public void cleanFutures() {
        Iterator<DelegatedRemoteTargetCreation> iCreations = delegatedRemoteTargetCreations.iterator();
        while (iCreations.hasNext()) {
            DelegatedRemoteTargetCreation creation = iCreations.next();
            if (shouldClean(creation)) {
                ComposedNode node = repository.get(ComposedNode.class, creation.getNodeId());
                nodeComposer.handleRemoteTargetCreationFailure(node);
                iCreations.remove();
            }
        }
    }

    public void registerDelegatedRequest(Id nodeId, Future<URI> futureTargetUri) {
        delegatedRemoteTargetCreations.add(new DelegatedRemoteTargetCreation(nodeId, futureTargetUri));
    }

    @TransactionAttribute(value = TransactionAttributeType.REQUIRES_NEW)
    public void handle(@Observes(during = TransactionPhase.AFTER_SUCCESS) RemoteTargetCreated remoteTargetCreated) {
        RemoteTarget remoteTarget = repository.get(RemoteTarget.class, remoteTargetCreated.getRemoteTarget().getId());
        URI remoteTargetUri = remoteTarget.getStorageService().getService().getBaseUri().resolve(remoteTarget.getSourceUri());

        Iterator<DelegatedRemoteTargetCreation> iCreations = delegatedRemoteTargetCreations.iterator();
        while (iCreations.hasNext()) {
            DelegatedRemoteTargetCreation creation = iCreations.next();
            Future<URI> futureTargetUri = creation.getFutureTargetUri();
            ComposedNode node = repository.get(ComposedNode.class, creation.getNodeId());

            if (tryToAllocate(node, futureTargetUri, remoteTarget, remoteTargetUri)) {
                iCreations.remove();
                break;
            }
        }
    }

    private boolean tryToAllocate(ComposedNode node, Future<URI> futureTargetUri, RemoteTarget remoteTarget, URI remoteTargetUri) {
        if (futureTargetUri.isDone()) {
            try {
                URI uri = futureTargetUri.get();

                if (uri.equals(remoteTargetUri)) {
                    logger.i("FUTURE Allocated {} {}", node.getId(), uri);
                    nodeComposer.allocateRemoteTarget(node, remoteTarget);
                    return true;
                }
            } catch (InterruptedException | ExecutionException e) {
                logger.e("Future error {}", e);
            }
        }
        return false;
    }

    private boolean shouldClean(DelegatedRemoteTargetCreation creation) {
        if (creation.getFutureTargetUri().isDone()) {
            try {
                creation.getFutureTargetUri().get();
            } catch (InterruptedException | ExecutionException e) {
                return true;
            }
        }

        return false;
    }
}
