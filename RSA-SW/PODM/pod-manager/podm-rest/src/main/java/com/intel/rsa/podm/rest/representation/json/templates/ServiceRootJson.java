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
import com.intel.rsa.podm.rest.odataid.ODataId;

import java.time.OffsetDateTime;
import java.util.UUID;

@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified", "time", "serviceVersion", "UUID", "links"
})
public final class ServiceRootJson extends BaseJson {
    public String id;
    public String name;
    public OffsetDateTime time;
    public String serviceVersion;
    @JsonProperty("UUID")
    public UUID uuid;
    public final Links links = new Links();

    public ServiceRootJson() {
        super("#RSAPODMServiceRoot.1.0.0.RSAPODMServiceRoot");
    }

    @JsonPropertyOrder({"chassis", "services", "systems", "managers"})
    public static final class Links {
        public ODataId chassis;
        public ODataId services;
        public ODataId systems;
        public ODataId managers;
    }
}

