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

import java.nio.file.Paths;
import java.util.HashSet;
import java.util.Set;

public final class ServiceEndpointCandidateSupplier {

    private ServiceEndpointCandidateSupplier() {
    }

    /**
     * Method returns a set of unique ServiceEndpointCandidates obtained from URI file sources.
     *
     * @return set of unique ServiceEndpointCandidates of service locations
     */
    public static Set<ServiceEndpointCandidate> getEndpointCandidates() {
        TmpLeasesFileReader tmpLeasesFileReader = new TmpLeasesFileReader(Paths.get("/tmp/leases"), new StringStreamProvider());
        ServiceListFileReader serviceListFileReader = new ServiceListFileReader(Paths.get("/tmp/services.list"), new StringStreamProvider());

        Set<ServiceEndpointCandidate> candidateSet = new HashSet<>();
        candidateSet.addAll(tmpLeasesFileReader.getEndpointCandidatesFromFile());
        candidateSet.addAll(serviceListFileReader.getPsmeUrisFromFile());

        return candidateSet;
    }

}
