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

package com.intel.rsa.client.webclient;

import com.intel.rsa.client.api.actions.RsaApiActionException;
import com.intel.rsa.client.api.reader.RsaApiReaderException;

import javax.ws.rs.core.Response;
import java.net.URI;
import java.util.HashMap;
import java.util.Map;

public final class CachedWebClient implements WebClient {
    private final WebClient decoratedClient;
    private final Object lock = new Object();

    private final Map<URI, Object> cache = new HashMap<>();


    public CachedWebClient(WebClient decoratedClient) {
        this.decoratedClient = decoratedClient;
    }


    @Override
    public <T> T getBase(Class<T> responseClass) throws RsaApiReaderException {
        return get(getBaseUri(), responseClass);
    }

    @Override
    public <T> T get(URI uri, Class<T> responseClass) throws RsaApiReaderException {
        synchronized (lock) {
            if (cache.containsKey(uri)) {
                return (T) cache.get(uri);
            }

            T response = decoratedClient.get(uri, responseClass);
            cache.put(uri, response);

            return response;
        }
    }

    @Override
    public void close() {
        synchronized (lock) {
            cache.clear();
            decoratedClient.close();
        }
    }

    @Override
    public <T> Response patch(URI requestUri, T obj) throws RsaApiActionException {
        return decoratedClient.patch(requestUri, obj);
    }

    @Override
    public <T> Response post(URI requestUri, T obj) throws RsaApiActionException {
        return decoratedClient.post(requestUri, obj);
    }

    @Override
    public URI getBaseUri() {
        return decoratedClient.getBaseUri();
    }
}
