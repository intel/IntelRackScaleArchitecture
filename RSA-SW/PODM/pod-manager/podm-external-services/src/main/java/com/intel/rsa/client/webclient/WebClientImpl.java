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

import javax.ws.rs.ProcessingException;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.client.Client;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriBuilder;
import java.net.URI;

/**
 * Web client for RSA REST API
 * Each instance of WebClient uses one JAX-RS client for all its requests
 */
public class WebClientImpl implements WebClient {
    private static final String PATCH = "PATCH";
    private final URI baseUri;
    private final Client client;

    public WebClientImpl(URI baseUri) {
        this.baseUri = baseUri;
        client = ResteasyClientProvider.getClientWithJacksonProvider();
    }

    /**
     * Creates HTTP GET Request for base uri related to this client,
     * waits for response and then deserializes it to T.
     *
     * @param tClass
     * @return
     * @throws com.intel.rsa.client.api.reader.RsaApiReaderException
     */
    @Override
    public <T> T getBase(Class<T> tClass) throws RsaApiReaderException {
        return doGetRequest(baseUri, tClass);
    }

    /**
     * Creates HTTP GET Request, waits for response and deserializes it to T
     * Request Uri is built based on baseUri (host, scheme, port) and provided uri (rest of uri)
     * @param <T>  type to which response is deserialized
     * @param uri
     * @param tClass
     * @return deserialized response
     * @throws com.intel.rsa.client.api.reader.RsaApiReaderException
     */
    @Override
    public <T> T get(URI uri, Class<T> tClass) throws RsaApiReaderException {
        URI requestUri = buildUri(uri);
        return doGetRequest(requestUri, tClass);
    }


    /**
     * Releases all resources related to this WebClient
     */
    @Override
    public void close() {
        client.close();
    }

    @Override
    public URI getBaseUri() {
        return baseUri;
    }

    private  <T> T doGetRequest(URI requestUri, Class<T> tClass) throws RsaApiReaderException {

        try {
            return client
               .target(requestUri)
               .request()
               .get(tClass);
        } catch (ProcessingException e) {
            throw new RsaApiReaderException(e.getMessage(), requestUri, e.getCause());
        } catch (WebApplicationException e) {
            throw new RsaApiReaderException(e.getMessage(), requestUri, e.getCause());
        }
    }

    /**
     * Creates HTTP PATCH Request with obj serialized to JSON as its body
     * Request Uri is built based on baseUri (host, scheme, port) and provided uri (rest of uri)
     * @param <T>
     * @param uri
     * @param obj
     * @throws RsaApiActionException
     */
    @Override
    public <T> Response patch(URI uri, T obj) throws RsaApiActionException {
        validateParameters(uri, obj);
        URI requestUri = buildUri(uri);
        try {
            return client.target(requestUri)
                    .request()
                    .build(PATCH, Entity.json(obj))
                    .invoke();
        } catch (ProcessingException e) {
            throw new RsaApiActionException(e.getMessage(), requestUri, e.getCause());
        }
    }

    /**
     * Creates HTTP POST Request with obj serialized to JSON as its body
     * Request Uri is built based on baseUri (host, scheme, port) and provided uri (rest of uri)
     * @param uri
     * @param obj
     * @param <T>
     * @return
     * @throws RsaApiActionException
     */
    @Override
    public <T> Response post(URI uri, T obj) throws RsaApiActionException {
        validateParameters(uri, obj);
        URI requestUri = buildUri(uri);
        try {
            return client.target(requestUri)
                    .request()
                    .post(Entity.json(obj));

        } catch (ProcessingException e) {
            throw new RsaApiActionException(e.getMessage(), requestUri, e.getCause());
        }
    }

    private <T> void validateParameters(URI requestUri, T obj) {
        if (requestUri == null) {
            throw new IllegalArgumentException("requestUri should not be null");
        }

        if (obj == null) {
            throw new IllegalArgumentException("obj should not be null");
        }
    }

    private URI buildUri(URI uri) {
        return UriBuilder.fromUri(uri)
                         .host(baseUri.getHost())
                         .scheme(baseUri.getScheme())
                         .port(baseUri.getPort())
                         .build();
    }
}
