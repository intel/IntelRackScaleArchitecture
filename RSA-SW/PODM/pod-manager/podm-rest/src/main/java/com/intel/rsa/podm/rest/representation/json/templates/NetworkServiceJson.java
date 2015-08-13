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
import com.intel.rsa.podm.rest.representation.json.templates.attributes.NetworkProtocolJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

@JsonPropertyOrder({
        "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified"
})
public class NetworkServiceJson extends BaseJson {
    public String id;
    public String name;
    public String description;
    public final StatusJson status = new StatusJson();
    public String hostName;
    @JsonProperty("FQDN")
    public String fqdn;
    @JsonProperty("HTTP")
    public NetworkProtocolJson http;
    @JsonProperty("HTTPS")
    public NetworkProtocolJson https;
    @JsonProperty("IPMI")
    public NetworkProtocolJson ipmi;
    @JsonProperty("SSH")
    public NetworkProtocolJson ssh;
    @JsonProperty("SNMP")
    public NetworkProtocolJson snmp;
    @JsonProperty("VirtualMedia")
    public NetworkProtocolJson virtualMedia;
    @JsonProperty("SSDP")
    public NetworkProtocolJson ssdp;
    @JsonProperty("Telnet")
    public NetworkProtocolJson telnet;
    @JsonProperty("KVMIP")
    public NetworkProtocolJson kvmip;

    public NetworkServiceJson() {
        super("#ManagerNetworkService.1.0.0.ManagerNetworkService");
    }

}
