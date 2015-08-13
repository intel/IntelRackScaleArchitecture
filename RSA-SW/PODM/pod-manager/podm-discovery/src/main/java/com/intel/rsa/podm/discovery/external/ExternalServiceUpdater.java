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

package com.intel.rsa.podm.discovery.external;

import com.intel.rsa.podm.business.entities.assets.ExternalService;
import com.intel.rsa.podm.services.detection.api.ServiceEndpoint;
import com.intel.rsa.podm.services.detection.api.ServiceType;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import javax.transaction.Transactional;

import static javax.transaction.Transactional.TxType.REQUIRES_NEW;

@Transactional(REQUIRES_NEW)
@Dependent
public class ExternalServiceUpdater {
    @Inject
    private ExternalServiceRepository repository;

    public void updateExternalService(ServiceEndpoint serviceEndpoint) {
        ExternalService service = repository.findOrNull(serviceEndpoint.getServiceUUID());

        ExternalService.Type serviceType = getServiceType(serviceEndpoint);

        if (service == null) {
            service = repository.create(serviceEndpoint.getServiceUUID(), serviceType);
        }

        service.setBaseUri(serviceEndpoint.getEndpointUri());
    }

    private ExternalService.Type getServiceType(ServiceEndpoint serviceEndpoint) {
        ServiceType serviceType = serviceEndpoint.getServiceType();

        switch (serviceType) {
            case PSME_SERVICE:
                return ExternalService.Type.PSME;
            case STORAGE_SERVICE:
                return ExternalService.Type.RSS;
            default:
                throw new UnsupportedOperationException("Service type: " + serviceType + " is not supported.");
        }
    }
}
