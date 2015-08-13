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

package com.intel.rsa.podm.discovery.internal;

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.PodCollection;
import com.intel.rsa.podm.business.entities.assets.PodCollectionProvider;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;

import javax.enterprise.context.ApplicationScoped;
import javax.inject.Inject;

import static com.google.common.base.Preconditions.checkNotNull;
import static com.intel.rsa.common.types.locations.Locations.podLocation;

/**
 * Handles Pod creation.
 */
@ApplicationScoped
public class PodDiscoveryHandler {
    private static final String RSA_POD_DEFAULT_NAME = "RSA POD";

    @Inject
    private Logger logger;

    @Inject
    private PodCollectionProvider podCollectionProvider;

    @Inject
    private DomainObjectRepository repository;


    public Pod createPod(Integer locationCoord) {
        checkNotNull(locationCoord);
        logger.d("Creating POD at location {}", locationCoord);
        Location podLocation = podLocation(locationCoord);

        PodCollection podCollection = podCollectionProvider.getInstance();
        Pod pod = podCollection.getByLocation(podLocation);
        if (pod != null) {
            throw new RuntimeException("Pod already exists under location:" + locationCoord);
        }
        pod = repository.create(Pod.class);
        pod.setName(RSA_POD_DEFAULT_NAME);
        pod.updateModified();
        pod.setLocation(podLocation);

        //TODO: Provide real data when possible
        pod.setState(State.ENABLED);
        pod.setHealth(Health.OK);
        pod.setEnumStatus(EnumStatus.ENUMERATED);

        podCollection.link(pod);
        return pod;
    }
}
