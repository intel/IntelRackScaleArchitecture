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

import java.io.IOException;
import java.net.URI;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.stream.Stream;

import static com.google.common.net.InetAddresses.isInetAddress;
import static com.intel.rsa.podm.services.detection.filesystem.UriHelper.isValidUrl;
import static javax.ws.rs.core.UriBuilder.fromUri;

/**
 * Class responsible for obtaining URIs of service endpoints
 * from file created during DHCP discovery of RSA components.
 */
class TmpLeasesFileReader {

    private static final Logger LOGGER = LoggerFactory.getLogger(TmpLeasesFileReader.class);

    private static final int TMP_LEASES_MIN_PARTS_LENGTH = 3;

    private static final int TMP_LEASES_IP_POSITION = 1;
    private static final int TMP_LEASES_TYPE_POSITION = 2;

    private StringStreamProvider stringStreamProvider;
    private Path path;

    public TmpLeasesFileReader(Path path, StringStreamProvider provider) {
        this.path = path;
        this.stringStreamProvider = provider;
    }

    /**
     * Method reads the contents of a file located at specified Path assuming
     * that each line consists of four single-whitespace separated parts:
     *
     * [MAC-address] [IP-address] [Service-type] [Location-id]
     * e.g.:
     * 00:00:00:00:00:00 10.0.0.1 rsa-tc .1.1.0
     *
     * Each line that has a valid number of parts and represents "rsa-tc" or "iscsi" service type
     * is transformed to a single service URI matching the template "http://{ip}:{port}/rest/v1",
     * where port number is always assumed to be default for given service type.
     *
     * Current defaults are:
     * rsa-tc is assumed to be psme service with 8888 default port
     * iscsi is assumed to be storage service with 7778 default port
     *
     * @return Collection of obatined ServiceEndpointCandidates
     */
    public Collection<ServiceEndpointCandidate> getEndpointCandidatesFromFile() {
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
            if (parts.length >= TMP_LEASES_MIN_PARTS_LENGTH) {
                String ip = ipFromParts(parts);
                TmpLeasesRecordType recordType = recordTypeFromParts(parts);

                if (isRecordTypeValid(recordType) && isInetAddress(ip)) {
                    tryAddEndpointCandidate(uris, ip, recordType);
                } else {
                    LOGGER.w("Error while parsing line with record: {}", record);
                }
            }
        });
        return uris;
    }

    private TmpLeasesRecordType recordTypeFromParts(String[] parts) {
        String typePart = parts[TMP_LEASES_TYPE_POSITION];
        return TmpLeasesRecordType.fromString(typePart.toLowerCase());
    }

    private String ipFromParts(String[] parts) {
        return parts[TMP_LEASES_IP_POSITION];
    }

    private void tryAddEndpointCandidate(Collection<ServiceEndpointCandidate> uris, String ip, TmpLeasesRecordType recordType) {
        URI uriCandidate = buildUri(ip, recordType);
        if (isValidUrl(uriCandidate)) {
            uris.add(new  ServiceEndpointCandidate(recordType.getApiType(), uriCandidate));
        }
    }

    private URI buildUri(String ip, TmpLeasesRecordType recordType) {
        return fromUri("http://{ip}:{port}/rest/v1")
                .resolveTemplate("ip", ip)
                .resolveTemplate("port", recordType.getDefaultPort())
                .build();
    }

    private boolean isRecordTypeValid(TmpLeasesRecordType recordType) {
        return recordType != TmpLeasesRecordType.UNKNOWN_RECORD;
    }
}
