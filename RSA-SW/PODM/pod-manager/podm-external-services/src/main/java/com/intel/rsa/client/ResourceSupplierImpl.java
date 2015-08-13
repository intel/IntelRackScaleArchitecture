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

import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.webclient.WebClient;

import java.net.URI;


/**
 * Generic ResourceSupplier implementation that uses HTTP to lazy load Resources
 * @param <T>
 */
public final class ResourceSupplierImpl<T extends ResourceImpl> implements ResourceSupplier<T> {
    private final URI uri;
    private final WebClient webClient;
    private Class<T> implClass;

    public ResourceSupplierImpl(WebClient webClient, URI uri, Class<T> implClass) {
        this.webClient = webClient;
        this.uri = uri;
        this.implClass = implClass;
    }

    @Override
    public T get() throws RsaApiReaderException {
        T resource = webClient.get(uri, implClass);
        resource.setWebClient(webClient);
        resource.setUri(uri);

        return resource;
    }
}
