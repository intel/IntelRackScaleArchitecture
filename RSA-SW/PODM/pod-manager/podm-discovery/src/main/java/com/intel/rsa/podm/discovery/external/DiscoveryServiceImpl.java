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

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.podm.services.detection.api.DiscoveryService;
import com.intel.rsa.podm.services.detection.api.ServiceEndpoint;

import javax.enterprise.context.ApplicationScoped;
import javax.inject.Inject;

@ApplicationScoped
public class DiscoveryServiceImpl implements DiscoveryService {
    @Inject
    private Logger logger;

    @Inject
    private DiscoveryScheduler scheduler;

    @Inject
    private ExternalServiceUpdater externalServiceUpdater;

    @Override
    public void discover(ServiceEndpoint serviceEndpoint) {
        logger.i("Discovery {} started", serviceEndpoint);
        externalServiceUpdater.updateExternalService(serviceEndpoint);
        scheduler.schedule(serviceEndpoint.getServiceUUID());
    }
}
