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

package com.intel.rsa.podm.discovery.external.linker;

import com.intel.rsa.podm.business.entities.assets.Drawer;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.ManagerCollectionProvider;
import com.intel.rsa.podm.business.entities.assets.Rack;
import com.intel.rsa.podm.business.entities.assets.StorageService;
import com.intel.rsa.podm.business.entities.assets.StorageServiceCollectionProvider;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.discovery.external.finders.RackFinder;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.Collection;

import static java.lang.String.format;

/**
 * Allows to associate two domain objects based on name of relationship
 */
@Dependent
public class DomainObjectLinker {
    @Inject
    private RackFinder rackFinder;

    @Inject
    private StorageServiceCollectionProvider serviceCollectionProvider;

    @Inject
    private ManagerCollectionProvider managerCollectionProvider;

    @Inject
    private Collection<Linker> linkers;

    public void link(DomainObject source, DomainObject target, String linkName) {
        for (Linker linker : linkers) {
            if (linker.tryLink(source, target, linkName))  {
                return;
            }
        }

        String msg = format("Cannot create link with name '%s' from %s to %s", linkName, source, target);
        throw new IllegalArgumentException(msg);
    }

    public void linkToDomainModel(DomainObject domainObject) {
        if (domainObject == null) {
            throw new IllegalArgumentException("domainObject must not be null");
        }

        if (domainObject instanceof Drawer) {
            linkDrawer((Drawer) domainObject);
        } else if (domainObject instanceof StorageService) {
            linkStorageService((StorageService) domainObject);
        } else if (domainObject instanceof Manager) {
            linkManager((Manager) domainObject);
        }
    }

    private void linkManager(Manager manager) {
        link(managerCollectionProvider.getInstance(), manager, "contains");
    }

    private void linkStorageService(StorageService storageService) {
        link(serviceCollectionProvider.getInstance(), storageService, "contains");
    }

    private void linkDrawer(Drawer drawer) {
        Rack rack = rackFinder.find(drawer.getLocation());
        link(rack, drawer, "contains");
    }

}
