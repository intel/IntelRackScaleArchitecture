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

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.common.logger.LoggerFactory;
import com.intel.rsa.podm.services.detection.api.ServiceEndpointCandidate;
import com.intel.rsa.podm.services.detection.api.ServiceType;

import javax.ws.rs.core.UriBuilder;
import java.io.IOException;
import java.net.URI;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.stream.Stream;

/**
 * Class used to provide additional service endpoints for service detection process
 */
class ServiceListFileReader {

    private static final Logger LOGGER = LoggerFactory.getLogger(ServiceListFileReader.class);

    private static final int SERVICE_LIST_MIN_PARTS_LENGTH = 1;

    private static final int SERVICE_LIST_URI_POSITION = 0;
    private static final int SERVICE_LIST_TYPE_POSITION = 1;

    private StringStreamProvider stringStreamProvider;
    private Path path;

    public ServiceListFileReader(Path path, StringStreamProvider provider) {
        this.path = path;
        this.stringStreamProvider = provider;
    }

    /**
     * Method reads the contents of a file located at specified Path assuming that each line
     * contains a full URI of a single service endpoint, e.g. http://127.0.0.1:8888/rest/v1
     * and optionally its type. If no type is given, PSME type is assumed.
     *
     * @return  Collection of obtained ServiceEndpointCandidates
     */
    public Collection<ServiceEndpointCandidate> getPsmeUrisFromFile() {
        try (Stream<String> records = stringStreamProvider.getStreamFromPath(path)) {
            return parseStringStream(records);
        } catch (IOException ioe) {
            LOGGER.d("Error while reading file: {}", path);
        }
        return Collections.emptyList();
    }

    private Collection<ServiceEndpointCandidate> parseStringStream(Stream<String> input) {
        Collection<ServiceEndpointCandidate> uris = new ArrayList<>();
        input.forEach(record -> {
            final String[] parts = record.split(" ");
            URI uri = UriBuilder.fromUri(uriFromParts(parts)).build();
            if (UriHelper.isValidUrl(uri)) {
                if (parts.length > SERVICE_LIST_MIN_PARTS_LENGTH) {
                    ServiceListRecordType recordType = recordTypeFromParts(parts);
                    if (recordType != ServiceListRecordType.UNKNOWN_RECORD) {
                        uris.add(new ServiceEndpointCandidate(recordType.getApiType(), uri));
                    }
                } else {
                    uris.add(new ServiceEndpointCandidate(ServiceType.PSME_SERVICE, uri));
                }
            }

        });
        return uris;
    }

    private ServiceListRecordType recordTypeFromParts(String[] parts) {
        String typePart = parts[SERVICE_LIST_TYPE_POSITION];
        return ServiceListRecordType.fromString(typePart.toLowerCase());
    }

    private String uriFromParts(String[] parts) {
        return parts[SERVICE_LIST_URI_POSITION];
    }

}
