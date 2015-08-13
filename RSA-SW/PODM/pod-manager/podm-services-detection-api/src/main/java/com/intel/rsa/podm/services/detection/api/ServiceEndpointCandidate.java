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

package com.intel.rsa.podm.services.detection.api;

import java.net.URI;

public class ServiceEndpointCandidate {

    private final ServiceType serviceType;
    private final URI endpointUri;

    public ServiceEndpointCandidate(ServiceType serviceType, URI endpointUri) {
        if (serviceType == null || endpointUri == null) {
            throw new IllegalArgumentException("ServiceType and URI cannot be null in ServiceEndpointCandidate");
        }
        this.serviceType = serviceType;
        this.endpointUri = endpointUri;
    }

    public URI getEndpointUri() {
        return endpointUri;
    }

    public ServiceType getServiceType() {
        return serviceType;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        ServiceEndpointCandidate candidate = (ServiceEndpointCandidate) o;

        if (serviceType != candidate.serviceType) {
            return false;
        }
        return endpointUri.equals(candidate.endpointUri);

    }

    @Override
    public int hashCode() {
        int result = serviceType.hashCode();
        result = 31 * result + endpointUri.hashCode();
        return result;
    }

    @Override
    public String toString() {
        return "ServiceEndpointCandidate{" + serviceType + ',' +  endpointUri + '}';
    }
}
