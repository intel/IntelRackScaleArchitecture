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

package com.intel.rsa.podm.discovery.external.mapper.rss;

import com.intel.rsa.client.api.reader.rss.IscsiAddress;
import com.intel.rsa.client.api.reader.rss.IscsiInitiator;
import com.intel.rsa.client.api.reader.rss.RssRemoteTarget;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.assets.RemoteTargetIscsiAddress;
import com.intel.rsa.podm.business.entities.assets.RemoteTargetIscsiInitiator;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.discovery.external.mapper.DomainObjectMapper;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.Objects;

@Dependent
public class RemoteTargetMapper extends DomainObjectMapper<RssRemoteTarget, RemoteTarget> {

    @Inject
    private DomainObjectRepository repository;

    public RemoteTargetMapper() {
        super(RssRemoteTarget.class, RemoteTarget.class);
        useLooseMatchingStrategy();
        registerProvider(RemoteTargetIscsiAddress.class, this::provideRemoteTargetIscsiAddress);
        registerProvider(RemoteTargetIscsiInitiator.class, this::provideRemoteTargetIscsiInitiator);
    }

    @Override
    protected void performNotAutomatedMapping(RssRemoteTarget source, RemoteTarget target) {
        target.updateModified();
    }

    private RemoteTargetIscsiAddress provideRemoteTargetIscsiAddress(IscsiAddress iscsiAddress) {
        for (RemoteTargetIscsiAddress addr : target.getRemoteTargetIscsiAddresses()) {
            if (Objects.equals(addr.getTargetIqn(), iscsiAddress.getTargetIQN())
                    && Objects.equals(addr.getTargetPortalIp(), iscsiAddress.getTargetPortalIP())
                    && Objects.equals(addr.getTargetPortalPort(), iscsiAddress.getTargetPortalPort())
                    && Objects.equals(addr.getTargetLun(), iscsiAddress.getTargetLUN())) {
                return addr;
            }
        }

        return repository.create(RemoteTargetIscsiAddress.class);
    }

    private RemoteTargetIscsiInitiator provideRemoteTargetIscsiInitiator(IscsiInitiator iscsiInitiator) {
        for (RemoteTargetIscsiInitiator initiator : target.getRemoteTargetIscsiInitiators()) {
            if (Objects.equals(initiator.getInitiatorIqn(), iscsiInitiator.getInitiatorIqn())) {
                return initiator;
            }
        }

        return repository.create(RemoteTargetIscsiInitiator.class);
    }
}
