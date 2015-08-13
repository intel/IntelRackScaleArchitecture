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

package com.intel.rsa.podm.remotetargetsupplier;

import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.assets.RemoteTargetIscsiAddress;
import com.intel.rsa.podm.business.entities.assets.RemoteTargetIscsiInitiator;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.business.entities.components.ComposedNode;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import javax.transaction.Transactional;


import java.util.Collection;
import java.util.List;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static javax.transaction.Transactional.TxType.REQUIRES_NEW;

@Dependent
@Transactional(REQUIRES_NEW)
public class RemoteTargetDirectory {
    @Inject
    private DomainObjectRepository repository;

    public IpxeScript getRemoteTarget(String macAddress) throws RemoteTargetNotFoundException {
        if (macAddress == null) {
            throw new RemoteTargetNotFoundException();
        }

        NetworkInterface networkInterface = findNetworkInterface(macAddress.toUpperCase()); // todo fixme
        ComposedNode composedNode = findComposedNode(networkInterface);
        RemoteTarget remoteTarget = findRemoteTarget(composedNode);

        RemoteTargetIscsiInitiator initiator = findInitiator(remoteTarget);
        RemoteTargetIscsiAddress address = findAddress(remoteTarget);
        Integer lun = findLun(address);

        return IpxeScript
                .newBuilder()
                .initiatorIqn(initiator.getInitiatorIqn())
                .serverName(address.getTargetPortalIp())
                .port(address.getTargetPortalPort())
                .lun(lun)
                .targetName(address.getTargetIqn())
                .build();
    }

    private NetworkInterface findNetworkInterface(String macAddress) throws RemoteTargetNotFoundException {
        NetworkInterface networkInterface = repository.getSingleByProperty(NetworkInterface.class, NetworkInterface.MAC_ADDR, macAddress);

        if (networkInterface == null) {
            throw new RemoteTargetNotFoundException();
        }
        return networkInterface;
    }

    private ComposedNode findComposedNode(NetworkInterface networkInterface) throws RemoteTargetNotFoundException {
        if (networkInterface.getComposedNode() == null) {
            throw new RemoteTargetNotFoundException();
        }

        return networkInterface.getComposedNode();
    }

    private RemoteTarget findRemoteTarget(ComposedNode composedNode) throws RemoteTargetNotFoundException {
        Collection<RemoteTarget> remoteTargets = composedNode.getRemoteTargets();

        if (remoteTargets.size() != 1) {
            throw new RemoteTargetNotFoundException();
        }

        return single(remoteTargets);
    }

    private RemoteTargetIscsiInitiator findInitiator(RemoteTarget remoteTarget) throws RemoteTargetNotFoundException {
        Collection<RemoteTargetIscsiInitiator> initiators = remoteTarget.getRemoteTargetIscsiInitiators();

        if (initiators.size() != 1) {
            throw new RemoteTargetNotFoundException();
        }

        return single(initiators);
    }

    private RemoteTargetIscsiAddress findAddress(RemoteTarget remoteTarget) throws RemoteTargetNotFoundException {
        Collection<RemoteTargetIscsiAddress> addresses = remoteTarget.getRemoteTargetIscsiAddresses();

        if (addresses.size() != 1) {
            throw new RemoteTargetNotFoundException();
        }

        return single(addresses);
    }

    private Integer findLun(RemoteTargetIscsiAddress address) throws RemoteTargetNotFoundException {
        List<Integer> luns = address.getTargetLun();

        if (luns.size() != 1) {
            throw new RemoteTargetNotFoundException();
        }

        return single(luns);
    }
}
