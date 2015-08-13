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

import com.intel.rsa.client.api.reader.RsaResource;
import com.intel.rsa.client.api.reader.RsaServiceRoot;
import com.intel.rsa.podm.business.entities.assets.ExternalService;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.discovery.external.linker.DomainObjectLinker;
import com.intel.rsa.podm.discovery.external.mapper.DomainObjectMultiMapper;
import com.intel.rsa.podm.discovery.external.restgraph.ResourceLink;
import com.intel.rsa.podm.discovery.external.restgraph.RestGraph;

import javax.ejb.AccessTimeout;
import javax.ejb.Singleton;
import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Map;
import java.util.UUID;

import static java.util.concurrent.TimeUnit.MINUTES;
import static javax.transaction.Transactional.TxType.REQUIRES_NEW;

@Singleton
@Transactional(REQUIRES_NEW)
public class DomainObjectGraphMapper {
    @Inject
    private DomainObjectMultiMapper multiMapper;

    @Inject
    private DomainObjectLinker linker;

    @Inject
    private ExternalServiceRepository repository;

    @Inject
    private DiscoveryNotifier discoveryNotifier;

    // TODO we should consider to provide retrying strategy
    @AccessTimeout(value = 5, unit = MINUTES)
    public void map(RestGraph graph) {
        UUID uuid = findUuid(graph);

        ExternalService service = repository.find(uuid);

        Map<RsaResource, DomainObject> map = multiMapper.map(graph.getResources(), service);

        for (ResourceLink link : graph.getLinks()) {
            String linkName = link.getName();
            DomainObject source = map.get(link.getSource());
            DomainObject target = map.get(link.getTarget());

            // Resources like ServiceRoot are not mapped to DomainObject
            if (source != null) {
                linker.link(source, target, linkName);
            }
        }

        for (DomainObject domainObject : map.values()) {
            if (domainObject != null) {
                linker.linkToDomainModel(domainObject);
                discoveryNotifier.notify(domainObject, service);
            }
        }
    }

    private UUID findUuid(RestGraph graph) {
        for (RsaResource resource : graph.getResources()) {
            if (resource instanceof RsaServiceRoot) {
                return ((RsaServiceRoot) resource).getUuid();
            }
        }

        throw new IllegalStateException("uuid was not found");
    }
}
