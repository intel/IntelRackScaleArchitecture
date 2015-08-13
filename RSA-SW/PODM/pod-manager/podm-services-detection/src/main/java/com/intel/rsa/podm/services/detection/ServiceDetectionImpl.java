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

package com.intel.rsa.podm.services.detection;

import com.intel.rsa.podm.services.detection.api.ServiceDetection;
import com.intel.rsa.podm.services.detection.api.ServiceDetectionException;
import com.intel.rsa.podm.services.detection.api.ServiceEndpoint;
import com.intel.rsa.podm.services.detection.api.ServiceEndpointCandidate;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import java.util.UUID;

/**
 * Service Detection implementation based on obtaining UUID of service
 * being detected at given URI using REST client.
 */
@RequestScoped
public class ServiceDetectionImpl implements ServiceDetection {

    @Inject
    private ServiceRestClient serviceRestClient;

    @Override
    public ServiceEndpoint getServiceEndpoint(ServiceEndpointCandidate candidate) throws ServiceDetectionException {
        UUID serviceUUID = serviceRestClient.obtainServiceUuidFromCandidate(candidate);
        return new ServiceEndpoint(candidate.getServiceType(), serviceUUID, candidate.getEndpointUri());
    }
}
