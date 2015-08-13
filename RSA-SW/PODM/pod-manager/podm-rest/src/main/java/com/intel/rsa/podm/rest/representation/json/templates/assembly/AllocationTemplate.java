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

package com.intel.rsa.podm.rest.representation.json.templates.assembly;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.google.common.base.Optional;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.RemoteStorageBackend;
import com.intel.rsa.common.types.RemoteStorageType;
import com.intel.rsa.common.types.ReplicationMethod;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.dto.RequestedLocalDrive;
import com.intel.rsa.podm.business.dto.RequestedMemory;
import com.intel.rsa.podm.business.dto.RequestedNetworkInterface;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.dto.RequestedRemoteDrive;
import com.intel.rsa.podm.business.dto.RequestedMasterDrive;
import com.intel.rsa.podm.business.dto.RequestedProcessor;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.ContextType;

import java.math.BigDecimal;
import java.net.URI;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.intel.rsa.podm.business.services.context.Context.contextOf;
import static java.util.Collections.emptyList;

@JsonIgnoreProperties(ignoreUnknown = false)
public final class AllocationTemplate {

    @JsonProperty("Systems")
    List<RequestedNodeImpl> systems;

    public List<RequestedNode> getSystems() {
        return Optional.<List>fromNullable(systems).or(emptyList());
    }
}

final class RequestedNodeImpl implements RequestedNode {
    @JsonProperty
    String name;
    @JsonProperty
    String description;
    @JsonProperty
    List<RequestedProcessor> processors;
    @JsonProperty
    List<RequestedMemory> memory;
    @JsonProperty
    List<RequestedRemoteDriveImpl> remoteDrives;
    @JsonProperty
    List<RequestedLocalDrive> localDrives;
    @JsonProperty
    List<RequestedNetworkInterface> networkInterfaces;

    @Override
    public String getName() {
        return name;
    }

    @Override
    public String getDescription() {
        return description;
    }

    @Override
    public List<RequestedProcessor> getProcessors() {
        return Optional.<List>fromNullable(processors).or(emptyList());
    }

    @Override
    public List<RequestedMemory> getMemory() {
        return Optional.<List>fromNullable(memory).or(emptyList());
    }

    @Override
    public List<RequestedRemoteDrive> getRemoteDrives() {
        return Optional.<List>fromNullable(remoteDrives).or(emptyList());
    }

    @Override
    public List<RequestedLocalDrive> getLocalDrives() {
        return Optional.<List>fromNullable(localDrives).or(emptyList());
    }

    @Override
    public List<RequestedNetworkInterface> getNetworkInterfaces() {
        return Optional.<List>fromNullable(networkInterfaces).or(emptyList());
    }

    @Override
    public Object getOem() {
        return new Object();
    }
}

final class RequestedRemoteDriveImpl implements RequestedRemoteDrive {
    @JsonProperty
    BigDecimal capacityGB;
    @JsonProperty
    RemoteStorageBackend storageBackend;
    @JsonProperty
    RemoteStorageType addressType;
    @JsonProperty
    String address;
    @JsonProperty
    RequestedMasterDriveImpl master;
    @JsonProperty
    Integer bootOrder;
    @JsonProperty
    Location location;

    @Override
    public BigDecimal getCapacityGB() {
        return capacityGB;
    }

    @Override
    public RemoteStorageBackend getStorageBackend() {
        return storageBackend;
    }

    @Override
    public RemoteStorageType getAddressType() {
        return addressType;
    }

    @Override
    public String getAddress() {
        return address;
    }

    @Override
    public RequestedMasterDriveImpl getMaster() {
        return master;
    }

    @Override
    public Integer getBootOrder() {
        return bootOrder;
    }

    @Override
    public Location getLocation() {
        return location;
    }
}

final class RequestedMasterDriveImpl implements RequestedMasterDrive {
    private static final Pattern LOGICAL_DRIVE_URI_PATTERN = Pattern.compile(
            "/rest/v1/Services/(?<service>\\d+)/LogicalDrives/(?<drive>\\d+)"
    );

    @JsonProperty
    ReplicationMethod type;
    @JsonProperty
    RemoteStorageType addressType;
    @JsonProperty
    Location location;

    Context logicalDrive;

    @Override
    public ReplicationMethod getType() {
        return type;
    }

    @Override
    public RemoteStorageType getAddressType() {
        return addressType;
    }

    @Override
    public Context getAddress() {
        return logicalDrive;
    }

    @Override
    public Location getLocation() {
        return location;
    }

    @JsonProperty
    private void setAddress(URI address) {
        if (address == null) {
            logicalDrive = null;
            return;
        }

        Matcher matcher = LOGICAL_DRIVE_URI_PATTERN.matcher(address.toString());
        if (!matcher.matches()) {
            throw new RuntimeException("Cannot parse address");
        }

        Id serviceId = Id.fromString(matcher.group("service"));
        Id driveId = Id.fromString(matcher.group("drive"));

        logicalDrive = contextOf(serviceId, ContextType.STORAGE_SERVICE)
                .child(driveId, ContextType.LOGICAL_DRIVE);
    }
}
