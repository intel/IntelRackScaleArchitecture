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

package com.intel.rsa.client;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.api.reader.ResourceLinks;
import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.reader.RsaResource;
import com.intel.rsa.client.api.reader.psme.PsmeResource;
import com.intel.rsa.client.dto.CollectionDto;
import com.intel.rsa.client.dto.ODataId;
import com.intel.rsa.client.webclient.WebClient;

import java.net.URI;
import java.util.ArrayList;
import java.util.Collection;

@JsonIgnoreProperties(ignoreUnknown = true)
public abstract class ResourceImpl implements PsmeResource, RsaResource {
    protected URI uri;
    protected WebClient webClient;
    @JsonProperty("Name")
    private String name;

    public void setUri(URI uri) {
        this.uri = uri;
    }

    public URI getUri() {
        return uri;
    }

    public String getName() {
        return name;
    }

    public void setWebClient(WebClient webClient) {
        this.webClient = webClient;
    }

    @Override
    public ResourceLinks getResourceLinks() {
        return new ResourceLinksImpl(this);
    }

    protected <T extends ResourceImpl>
    Iterable<ResourceSupplier<T>> processCollectionResource(ODataId collection, Class<T> memberClass) throws RsaApiReaderException {
        Iterable<ODataId> members = toSupplier(collection, CollectionDto.class)
                .get()
                .getLinks()
                .getMembers();

        return toSuppliers(members, memberClass);
    }

    protected <T extends ResourceImpl>
    Iterable<ResourceSupplier<T>> toSuppliers(Iterable<ODataId> collection, Class<T> resourceClass) throws RsaApiReaderException {
        if (collection == null) {
            throw new RsaApiReaderException("collection should not be null", getUri());
        }

        Collection<ResourceSupplier<T>> resourceSuppliers = new ArrayList<>();

        for (ODataId member : collection) {
            resourceSuppliers.add(toSupplier(member, resourceClass));
        }

        return resourceSuppliers;
    }

    protected <T extends ResourceImpl> ResourceSupplier<T> toSupplier(ODataId oDataId, Class<T> resourceClass) {
        if (oDataId == null || oDataId.toUri() == null) {
            throw new IllegalArgumentException("oDataId should not be null");
        }

        return new ResourceSupplierImpl<>(webClient, oDataId.toUri(), resourceClass);
    }
}
