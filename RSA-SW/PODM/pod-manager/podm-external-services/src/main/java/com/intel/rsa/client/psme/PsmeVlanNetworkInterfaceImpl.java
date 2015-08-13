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
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.api.reader.psme.PsmeVlanNetworkInterface;
import com.intel.rsa.client.api.reader.psme.PsmeVlanNetworkInterfaceIpV4Address;
import com.intel.rsa.client.api.reader.psme.PsmeVlanNetworkInterfaceIpV6Address;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;

import java.util.Collection;
import java.util.Collections;
import java.util.List;

public class PsmeVlanNetworkInterfaceImpl extends ResourceImpl implements PsmeVlanNetworkInterface  {
    @JsonProperty("Description")
    private String description;
    @JsonProperty ("FQDN")
    private String fqdn;
    @JsonProperty ("HostName")
    private String hostName;
    @JsonProperty ("Status")
    private StatusDto status;
    @JsonProperty("VLANEnable")
    private Boolean vlanEnable;
    @JsonProperty("VLANId")
    private Integer vlanId;
    @JsonProperty ("IPv4Address")
    private List<PsmeVlanNetworkInterfaceIpV4AddressImpl> iPv4Addresses;
    @JsonProperty ("IPv6Address")
    private List<PsmeVlanNetworkInterfaceIpV6AddressImpl> iPv6Addresses;

    @Override
    public Health getHealth() {
        return status.getHealth();
    }

    @Override
    public State getState() {
        return status.getState();
    }

    @Override
    public Boolean isVlanEnable() {
        return vlanEnable;
    }

    @Override
    public Integer getVlanId() {
        return vlanId;
    }

    @Override
    public String getDescription() {
        return description;
    }

    @Override
    public Collection<PsmeVlanNetworkInterfaceIpV4Address> getIpV4Addresses() {
        return Collections.unmodifiableCollection(iPv4Addresses);
    }

    @Override
    public Collection<PsmeVlanNetworkInterfaceIpV6Address> getIpV6Addresses() {
        return Collections.unmodifiableCollection(iPv6Addresses);
    }
}
