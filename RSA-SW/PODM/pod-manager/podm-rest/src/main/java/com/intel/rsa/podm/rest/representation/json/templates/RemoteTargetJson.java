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

package com.intel.rsa.podm.rest.representation.json.templates;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.IscsiAddressJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.IscsiInitiatorJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;


@JsonPropertyOrder({"@odata.context", "@odata.id", "@odata.type", "id", "name", "modified",
        "status", "enumStatus", "type", "Addresses", "Initiator"})
public class RemoteTargetJson extends BaseJson {

    public Id id;
    public String name;
    public final StatusJson status = new StatusJson();
    public String type;
    public EnumStatus enumStatus;
    public final Oem oem = new Oem();

    @JsonProperty("Addresses")
    public List<AddressWrapper> addresses = new ArrayList<>();

    @JsonProperty("Initiator")
    public List<InitiatorWrapper> initiator = new ArrayList<>();

    public Links links = new Links();

    public RemoteTargetJson() {
        super("#RSARemoteTarget.1.0.0.RSARemoteTarget");
    }

    @JsonPropertyOrder({"logicalDrives", "managedBy"})
    public static final class Links extends BaseLinksJson {
        public Collection<ODataId> logicalDrives = new ArrayList<>();
        public Collection<ODataId> managedBy = new ArrayList<>();
    }

    public static final class AddressWrapper {
        @JsonProperty("iSCSI")
        public IscsiAddressJson iSCSI = new IscsiAddressJson();
    }

    public static final class InitiatorWrapper {
        @JsonProperty("iSCSI")
        public IscsiInitiatorJson iSCSI = new IscsiInitiatorJson();
    }

    public static final class Oem {
    }
}
