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

public interface WebClient extends AutoCloseable {
    URI getBaseUri();
    <T> T getBase(Class<T> tClass) throws RsaApiReaderException;
    <T> T get(URI uri, Class<T> tClass) throws RsaApiReaderException;
    @Override
    void close();

    <T> Response patch(URI requestUri, T obj) throws RsaApiActionException;

    <T> Response post(URI requestUri, T obj) throws RsaApiActionException;
}
