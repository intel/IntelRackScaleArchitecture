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

package com.intel.rsa.client.psme;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.LinkName;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.reader.psme.PsmeFabricModule;
import com.intel.rsa.client.api.reader.psme.PsmeManager;
import com.intel.rsa.client.api.reader.psme.PsmeSwitch;
import com.intel.rsa.client.dto.LocationDto;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.client.psme.dto.FabricModuleLinksDto;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;

import static com.intel.rsa.common.types.locations.Locations.moduleLocation;

public class PsmeFabricModuleImpl extends ResourceImpl implements PsmeFabricModule {

    @JsonProperty ("Status")
    private StatusDto status;

    @JsonProperty ("Location")
    private LocationDto location;

    @JsonProperty("Links")
    private FabricModuleLinksDto links;

    @Override
    public State getState() {
        return status.getState();
    }

    @Override
    public Health getHealth() {
        return status.getHealth();
    }

    @Override
    public Location getLocation() {
        return moduleLocation(location.getPod(), location.getRack(), location.getDrawer(), location.getModule());
    }

    @Override
    @LinkName("switches")
    public Iterable<ResourceSupplier<PsmeSwitch>> getSwitches() throws RsaApiReaderException {
        return processCollectionResource(links.getSwitches(), (Class) PsmeSwitchImpl.class);
    }

    @Override
    @LinkName("managedBy")
    public Iterable<ResourceSupplier<PsmeManager>> getManagers() throws RsaApiReaderException {
        return toSuppliers(links.getManagers(), (Class) PsmeManagerImpl.class);
    }
}
