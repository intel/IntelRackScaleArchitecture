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
import java.util.UUID;

/**
 * Represents REST API entry point of external service
 */
public final class ServiceEndpoint {

    private final ServiceType serviceType;
    private final UUID serviceUUID;
    private final URI endpointUri;

    public ServiceEndpoint(ServiceType serviceType, UUID serviceUUID, URI endpointUri) {
        this.serviceType = serviceType;
        this.serviceUUID = serviceUUID;
        this.endpointUri = endpointUri;
    }

    public URI getEndpointUri() {
        return endpointUri;
    }

    public ServiceType getServiceType() {
        return serviceType;
    }

    public UUID getServiceUUID() {
        return serviceUUID;
    }

    @Override
    public String toString() {
        return String.format("{URI=%s, type=%s, UUID=%s}", endpointUri, serviceType, serviceUUID);
    }
}
