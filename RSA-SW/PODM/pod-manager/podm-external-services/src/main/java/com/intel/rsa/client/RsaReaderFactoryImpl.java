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

import com.intel.rsa.client.api.reader.RsaReaderFactory;
import com.intel.rsa.client.api.reader.psme.PsmeReader;
import com.intel.rsa.client.api.reader.rss.RssReader;
import com.intel.rsa.client.psme.PsmeReaderImpl;
import com.intel.rsa.client.rss.RssReaderImpl;
import com.intel.rsa.client.webclient.CachedWebClient;
import com.intel.rsa.client.webclient.WebClientImpl;

import javax.enterprise.context.Dependent;
import java.net.URI;

@Dependent
public class RsaReaderFactoryImpl implements RsaReaderFactory {
    @Override
    public RssReader createRssReader(URI baseUri) {
        CachedWebClient cachedWebClient = createWebClient(baseUri);
        return new RssReaderImpl(cachedWebClient);
    }

    @Override
    public PsmeReader createPsmeReader(URI baseUri) {
        CachedWebClient cachedWebClient = createWebClient(baseUri);
        return new PsmeReaderImpl(cachedWebClient);
    }

    private CachedWebClient createWebClient(URI baseUri) {
        WebClientImpl webClient = new WebClientImpl(baseUri);
        return new CachedWebClient(webClient);
    }
}
