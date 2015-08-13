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
import com.intel.rsa.client.api.reader.psme.PsmeSwitchPort;
import com.intel.rsa.client.api.reader.psme.PsmeVlanNetworkInterface;
import com.intel.rsa.client.psme.dto.PsmeSwitchPortLinksDto;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.AdministrativeState;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.OperationalState;
import com.intel.rsa.common.types.LinkType;
import com.intel.rsa.common.types.State;

@JsonIgnoreProperties(ignoreUnknown = true)
public class PsmeSwitchPortImpl extends ResourceImpl implements PsmeSwitchPort  {

    @JsonProperty("Status")
    private StatusDto status;

    @JsonProperty("LinkSpeedGbps")
    private Integer linkSpeedGbps;

    @JsonProperty("OperationalState")
    private OperationalState operationalState;

    @JsonProperty("AdministrativeState")
    private AdministrativeState administrativeState;

    @JsonProperty("LinkType")
    private LinkType linkType;

    @JsonProperty("NeighbourPort")
    private String neighbourPort;

    @JsonProperty("Links")
    private PsmeSwitchPortLinksDto links;

    @Override
    public Health getHealth() {
        return status.getHealth();
    }

    @Override
    public State getState() {
        return status.getState();
    }

    @Override
    public Health getHealthRollup() {
        return status.getHealthRollup();
    }

    @Override
    public OperationalState getOperationalState() {
        return operationalState;
    }

    @Override
    public AdministrativeState getAdministrativeState() {
        return administrativeState;
    }

    @Override
    public LinkType getLinkType() {
        return linkType;
    }

    @Override
    public String getNeighbourPort() {
        return neighbourPort;
    }

    @Override
    public Integer getLinkSpeedGbps() {
        return linkSpeedGbps;
    }

    @Override
    @LinkName("vlans")
    public Iterable<ResourceSupplier<PsmeVlanNetworkInterface>> getVlans() throws RsaApiReaderException {
        return processCollectionResource(links.getVlans(), (Class) PsmeVlanNetworkInterfaceImpl.class);
    }
}
