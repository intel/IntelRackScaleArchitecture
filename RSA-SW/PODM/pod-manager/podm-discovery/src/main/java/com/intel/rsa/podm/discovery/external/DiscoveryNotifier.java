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

package com.intel.rsa.podm.discovery.external;

import com.intel.rsa.podm.business.entities.assets.ExternalService;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.events.RemoteTargetCreated;

import javax.enterprise.context.ApplicationScoped;
import javax.enterprise.event.Event;
import javax.inject.Inject;

@ApplicationScoped
public class DiscoveryNotifier {
    @Inject
    private Event<RemoteTargetCreated> remoteTargetCreationNotifier;

    public void notify(DomainObject discoveredDomainObject, ExternalService externalService) {
        if (discoveredDomainObject instanceof RemoteTarget) { // TODO notify about new targets only
            remoteTargetCreationNotifier.fire(new RemoteTargetCreated((RemoteTarget) discoveredDomainObject));
        }
    }
}
