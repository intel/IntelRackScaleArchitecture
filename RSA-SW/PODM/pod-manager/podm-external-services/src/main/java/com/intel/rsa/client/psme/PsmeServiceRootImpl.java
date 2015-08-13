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
import com.intel.rsa.client.api.reader.psme.PsmeDrawer;
import com.intel.rsa.client.api.reader.psme.PsmeServiceRoot;
import com.intel.rsa.client.dto.ODataId;

import java.util.UUID;

@JsonIgnoreProperties(ignoreUnknown = true)
public class PsmeServiceRootImpl extends ResourceImpl implements PsmeServiceRoot {
    private static final ODataId DRAWER_COLLECTION = new ODataId("rest/v1/Drawers");

    @JsonProperty("UUID")
    private UUID uuid;

    @Override
    public UUID getUuid() {
        return uuid;
    }

    @Override
    @LinkName("drawers")
    public Iterable<ResourceSupplier<PsmeDrawer>> getDrawers() throws RsaApiReaderException {
        return processCollectionResource(DRAWER_COLLECTION, (Class) PsmeDrawerImpl.class);
    }
}
