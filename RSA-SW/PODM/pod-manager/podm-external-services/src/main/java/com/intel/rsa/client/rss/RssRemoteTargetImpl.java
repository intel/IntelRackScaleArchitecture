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

package com.intel.rsa.client.rss;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.intel.rsa.client.LinkName;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.reader.rss.IscsiAddress;
import com.intel.rsa.client.api.reader.rss.IscsiInitiator;
import com.intel.rsa.client.api.reader.rss.RssLogicalDrive;
import com.intel.rsa.client.api.reader.rss.RssRemoteTarget;
import com.intel.rsa.client.api.reader.rss.RssStorageManager;
import com.intel.rsa.client.dto.ODataId;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;

import java.util.List;
import java.util.stream.Collectors;

import static com.google.common.base.MoreObjects.toStringHelper;
import static java.util.Collections.emptyList;

@JsonIgnoreProperties(ignoreUnknown = true)
public class RssRemoteTargetImpl extends ResourceImpl implements RssRemoteTarget {

    @JsonProperty("Status")
    private StatusDto status = new StatusDto();

    @JsonProperty ("EnumStatus")
    private EnumStatus enumStatus;

    @JsonProperty ("Type")
    private String type;

    @JsonProperty("Addresses")
    private List<AddressWrapper> addresses = emptyList();

    @JsonProperty("Initiator")
    private List<InitiatorWrapper> initiator = emptyList();

    @JsonProperty("Links")
    private Links links = new Links();

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
    public EnumStatus getEnumStatus() {
        return enumStatus;
    }

    @Override
    public String getType() {
        return type;
    }

    @Override
    public List<IscsiAddress> getAddresses() {
        return addresses.stream().map((wrap) -> wrap.address).collect(Collectors.toList());
    }

    @Override
    public List<IscsiInitiator> getInitiator() {
        return initiator.stream().map((wrap) -> wrap.inititator).collect(Collectors.toList());
    }

    @LinkName("logicalDrives")
    @Override
    public Iterable<ResourceSupplier<RssLogicalDrive>> getLogicalDrives() throws RsaApiReaderException {
        return toSuppliers(links.logicalDrives, (Class) RssLogicalDriveImpl.class);
    }

    @LinkName("managedBy")
    @Override
    public Iterable<ResourceSupplier<RssStorageManager>> getManagers() throws RsaApiReaderException {
        return toSuppliers(links.managedBy, (Class) RssStorageManagerImpl.class);
    }

    @Override
    public String toString() {
        return toStringHelper(this).toString();
    }

    @JsonIgnoreProperties(ignoreUnknown = true)
    private static final class Links {
        @JsonProperty("LogicalDrives")
        private List<ODataId> logicalDrives = emptyList();
        @JsonProperty("ManagedBy")
        private List<ODataId> managedBy = emptyList();
    }

    public static final class AddressWrapper {
        @JsonProperty("iSCSI")
        @JsonDeserialize(as = IscsiAddressImpl.class)
        IscsiAddress address;
    }

    public static final class InitiatorWrapper {
        @JsonProperty("iSCSI")
        @JsonDeserialize(as = IscsiInitiatorImpl.class)
        IscsiInitiator inititator;
    }
}
