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

package com.intel.rsa.podm.services.detection.filesystem;

import com.intel.rsa.podm.services.detection.api.ServiceEndpointCandidate;
import com.intel.rsa.podm.services.detection.api.ServiceType;
import org.testng.annotations.BeforeMethod;
import org.testng.annotations.Test;

import java.io.IOException;
import java.net.URI;
import java.nio.file.Path;
import java.util.Collection;
import java.util.stream.Stream;

import static java.util.Arrays.asList;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.collection.IsIterableContainingInOrder.contains;

import static org.mockito.Matchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertTrue;

public class ServiceListFileReaderTest {

    private ServiceListFileReader reader;

    private Path path;
    private StringStreamProvider provider;

    @BeforeMethod
    public void before() {
        path = mock(Path.class);
        provider = mock(StringStreamProvider.class);

        reader = new ServiceListFileReader(path, provider);
    }

    @Test
    public void whenFileError_shouldReturnEmptyList() throws IOException {
        when(provider.getStreamFromPath(any())).thenThrow(IOException.class);

        Collection<ServiceEndpointCandidate> actual = reader.getPsmeUrisFromFile();
        assertTrue(actual.isEmpty());
    }

    @Test
    public void whenValidInputNoType_shouldReturnValidPsmeCandidate() throws IOException {
        Stream<String> answer = asList(TestData.VALID_URI_STRING_NO_TYPE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getPsmeUrisFromFile();
        assertThat(actual, contains(TestData.VALID_PSME_CANDIDATE));
    }

    @Test
    public void whenValidInputPsmeType_shouldReturnValidPsmeCandidate() throws IOException {
        Stream<String> answer = asList(TestData.VALID_URI_STRING_PSME_TYPE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getPsmeUrisFromFile();
        assertThat(actual, contains(TestData.VALID_PSME_CANDIDATE));
    }

    @Test
    public void whenValidInputRssType_shouldReturnValidRssCandidate() throws IOException {
        Stream<String> answer = asList(TestData.VALID_URI_STRING_PSME_TYPE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getPsmeUrisFromFile();
        assertThat(actual, contains(TestData.VALID_PSME_CANDIDATE));
    }

    @Test
    public void whenInvalidUri_shouldReturnEmptyList() throws IOException {
        Stream<String> answer = asList(TestData.JUNK_LINE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getPsmeUrisFromFile();
        assertTrue(actual.isEmpty());
    }

    @Test
    public void whenMixedInput_shouldHandleOnlyValidLines()
            throws IOException {
        Stream<String> answer = asList(
                TestData.VALID_URI_STRING_INVALID_TYPE,
                TestData.VALID_URI_STRING_NO_TYPE,
                TestData.VALID_URI_STRING_RSS_TYPE,
                TestData.VALID_URI_STRING_PSME_TYPE,
                TestData.JUNK_LINE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getPsmeUrisFromFile();
        assertThat(actual, contains(
                TestData.VALID_PSME_CANDIDATE,
                TestData.VALID_RSS_CANDIDATE,
                TestData.VALID_PSME_CANDIDATE));
    }

    static class TestData {
        static final String VALID_URI_STRING_NO_TYPE = "http://10.0.0.1:8888/rest/v1";
        static final String VALID_URI_STRING_PSME_TYPE = "http://10.0.0.1:8888/rest/v1 psme";
        static final String VALID_URI_STRING_RSS_TYPE = "http://10.0.0.1:8888/rest/v1 rss";
        static final String VALID_URI_STRING_INVALID_TYPE = "http://10.0.0.1:8888/rest/v1 zergrush";
        static final String JUNK_LINE = "@$%@$TERFVSDFG";
        static final ServiceEndpointCandidate VALID_PSME_CANDIDATE =
                new ServiceEndpointCandidate(ServiceType.PSME_SERVICE, URI.create(VALID_URI_STRING_NO_TYPE));
        static final ServiceEndpointCandidate VALID_RSS_CANDIDATE =
                new ServiceEndpointCandidate(ServiceType.STORAGE_SERVICE, URI.create(VALID_URI_STRING_NO_TYPE));
    }
}
