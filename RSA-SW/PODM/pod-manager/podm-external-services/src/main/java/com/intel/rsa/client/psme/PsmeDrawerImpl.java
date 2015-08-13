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

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.LinkName;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.reader.psme.PsmeComputeModule;
import com.intel.rsa.client.api.reader.psme.PsmeDrawer;
import com.intel.rsa.client.api.reader.psme.PsmeFabricModule;
import com.intel.rsa.client.api.reader.psme.PsmeManager;
import com.intel.rsa.client.psme.dto.DrawerLinksDto;
import com.intel.rsa.client.dto.LocationDto;
import com.intel.rsa.client.dto.ODataId;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;

import java.util.List;

import static com.intel.rsa.common.types.locations.Locations.drawerLocation;

@JsonIgnoreProperties(ignoreUnknown = true)
public class PsmeDrawerImpl extends ResourceImpl implements PsmeDrawer  {
    @JsonProperty ("Model")
    private String model;

    @JsonProperty ("Manufacturer")
    private String manufacturer;

    @JsonProperty ("SerialNumber")
    private String serialNumber;

    @JsonProperty ("Status")
    private StatusDto status;

    @JsonProperty ("EnumStatus")
    private EnumStatus enumStatus;

    @JsonProperty ("Location")
    private LocationDto location;

    @JsonProperty ("Links")
    private DrawerLinksDto links;

    @Override
    public String getModel() {
        return model;
    }

    @Override
    public String getManufacturer() {
        return manufacturer;
    }

    @Override
    public String getSerialNumber() {
        return serialNumber;
    }

    @Override
    public Location getLocation() {
        return drawerLocation(location.getPod(), location.getRack(), location.getDrawer());
    }

    @Override
    public State getState() {
        return status.getState();
    }

    @Override
    public Health getHealth() {
        return status.getHealth();
    }

    @Override
    public Health getHealthRollup() {
        return status.getHealthRollup();
    }

    @Override
    @LinkName("computeModules")
    public Iterable<ResourceSupplier<PsmeComputeModule>> getComputeModules() throws RsaApiReaderException {
        return processCollectionResource(links.getComputeModules(), (Class) PsmeComputeModuleImpl.class);
    }

    @Override
    @LinkName("fabricModules")
    public Iterable<ResourceSupplier<PsmeFabricModule>> getFabricModules() throws RsaApiReaderException {
        return processCollectionResource(links.getFabricModules(), (Class) PsmeFabricModuleImpl.class);
    }

    @Override
    @LinkName("managedBy")
    public Iterable<ResourceSupplier<PsmeManager>> getManagers() throws RsaApiReaderException {
        Class<? extends ResourceImpl> memberClass = PsmeManagerImpl.class;
        List<ODataId> members = links.getManagers();
        return toSuppliers(members, (Class) memberClass);
    }

    @Override
    public EnumStatus getEnumStatus() {
        return enumStatus;
    }

}
