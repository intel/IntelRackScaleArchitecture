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

public class TmpLeasesFileReaderTest {

    private TmpLeasesFileReader reader;

    private Path path;
    private StringStreamProvider provider;

    @BeforeMethod
    public void before() {
        path = mock(Path.class);
        provider = mock(StringStreamProvider.class);

        reader = new TmpLeasesFileReader(path, provider);
    }

    @Test
    public void whenFileError_shouldReturnEmptyList() throws IOException {
        when(provider.getStreamFromPath(any())).thenThrow(IOException.class);

        Collection<ServiceEndpointCandidate> actual = reader.getEndpointCandidatesFromFile();
        assertTrue(actual.isEmpty());
    }

    @Test
    public void whenValidPsmeLine_shouldReturnValidUri() throws IOException {
        Stream<String> answer = asList(TestData.VALID_PSME_LINE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getEndpointCandidatesFromFile();
        assertThat(actual, contains(TestData.VALID_PSME_CANDIDATE));
    }

    @Test
    public void whenValidStorageLine_shouldReturnValidUri() throws IOException {
        Stream<String> answer = asList(TestData.VALID_STORAGE_LINE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getEndpointCandidatesFromFile();
        assertThat(actual, contains(TestData.VALID_STORAGE_CANDIDATE));
    }

    @Test
    public void whenValidPsmeAndStorageLine_shouldReturnValidUris() throws IOException {
        Stream<String> answer = asList(TestData.VALID_PSME_LINE, TestData.VALID_STORAGE_LINE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getEndpointCandidatesFromFile();
        assertThat(actual, contains(TestData.VALID_PSME_CANDIDATE, TestData.VALID_STORAGE_CANDIDATE));
    }

    @Test
    public void whenJunk_shouldReturnEmptyList() throws IOException {
        Stream<String> answer = asList(TestData.JUNK_LINE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getEndpointCandidatesFromFile();
        assertTrue(actual.isEmpty());
    }

    @Test
    public void whenInvalidType_shouldReturnEmptyList() throws IOException {
        Stream<String> answer = asList(TestData.INVALID_TYPE_LINE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getEndpointCandidatesFromFile();
        assertTrue(actual.isEmpty());
    }

    @Test
    public void whenInvalidIp_shouldReturnEmptyList() throws IOException {
        Stream<String> answer = asList(TestData.INVALID_IP_LINE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getEndpointCandidatesFromFile();
        assertTrue(actual.isEmpty());
    }

    @Test
    public void whenMixedInput_shouldHandleOnlyValidLines() throws IOException {
        Stream<String> answer = asList(TestData.VALID_PSME_LINE, TestData.JUNK_LINE, TestData.VALID_STORAGE_LINE).stream();
        when(provider.getStreamFromPath(any())).thenReturn(answer);

        Collection<ServiceEndpointCandidate> actual = reader.getEndpointCandidatesFromFile();
        assertThat(actual, contains(TestData.VALID_PSME_CANDIDATE, TestData.VALID_STORAGE_CANDIDATE));
    }

    static class TestData {
        static final ServiceEndpointCandidate VALID_PSME_CANDIDATE =
                new ServiceEndpointCandidate(ServiceType.PSME_SERVICE, URI.create("http://10.0.0.1:8888/rest/v1"));
        static final ServiceEndpointCandidate VALID_STORAGE_CANDIDATE =
                new ServiceEndpointCandidate(ServiceType.STORAGE_SERVICE, URI.create("http://10.0.0.2:8888/rest/v1"));
        static final String VALID_PSME_LINE = "00:00:00:00:00:00 10.0.0.1 rsa-tc .1.1.2";
        static final String VALID_STORAGE_LINE = "00:00:00:00:00:00 10.0.0.2 iscsi .1.1.2";
        static final String INVALID_IP_LINE = "00:00:00:00:00:00 999.999.999.999 rsa-tc .1.1.2";
        static final String INVALID_TYPE_LINE = "00:00:00:00:00:00 10.0.0.1 blade .1.1.2";
        static final String JUNK_LINE = "@$%@$TERFVSDFG";
    }
}
