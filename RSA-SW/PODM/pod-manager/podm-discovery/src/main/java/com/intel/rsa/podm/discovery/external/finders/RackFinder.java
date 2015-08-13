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

package com.intel.rsa.podm.discovery.external.finders;


import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.PodCollectionProvider;
import com.intel.rsa.podm.business.entities.assets.Rack;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.HashMap;
import java.util.Map;

import static javax.transaction.Transactional.TxType.MANDATORY;

@Transactional(MANDATORY)
@Dependent
public class RackFinder {

    @Inject
    private DomainObjectRepository repository;

    @Inject
    private PodCollectionProvider podCollectionProvider;

    public Rack find(Location drawerLocation) {
        Map<String, Integer> rackLocationCoords = new HashMap<>(drawerLocation.toMap());
        rackLocationCoords.remove("Drawer");

        Location location = new Location(rackLocationCoords);

        Rack rack = repository.getSingleByProperty(Rack.class, Rack.LOCATION, location);

        if (rack == null) {
            rack = repository.create(Rack.class);
            Location podLocation = new Location("Pod=1");
            Pod pod = podCollectionProvider.getInstance().getByLocation(podLocation);
            pod.contain(rack);
            rack.setName("RSA Rack");
            rack.setLocation(location);
            rack.updateModified();
            rack.setState(State.ENABLED);
            rack.setHealth(Health.OK);
        }

        return rack;
    }
}
