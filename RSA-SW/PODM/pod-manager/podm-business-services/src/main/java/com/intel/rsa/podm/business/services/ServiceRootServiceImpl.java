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

package com.intel.rsa.podm.business.services;

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.podm.business.dto.ServiceRootDto;
import com.intel.rsa.podm.business.services.configuration.ServiceRootUuidProvider;

import javax.annotation.PostConstruct;
import javax.enterprise.context.ApplicationScoped;
import javax.inject.Inject;
import java.time.OffsetDateTime;
import java.util.UUID;

@ApplicationScoped
public class ServiceRootServiceImpl implements ServiceRootService {

    private static final String SERVICE_VERSION = "1.0.0";

    @Inject
    ServiceRootUuidProvider serviceRootUuidProvider;

    @Inject
    Logger logger;

    private UUID serviceRootUuid;
    private OffsetDateTime modified;

    @PostConstruct
    void initialize() {
        modified = OffsetDateTime.now();
        serviceRootUuid = serviceRootUuidProvider.getUuid();
    }

    @Override
    public ServiceRootDto getServiceRoot() {
        return ServiceRootDto
                .newBuilder()
                .id("RootService")
                .modified(modified)
                .time(OffsetDateTime.now())
                .serviceVersion(SERVICE_VERSION)
                .uuid(serviceRootUuid)
                .name("RSA POD Manager Service Root")
                .build();
    }
}
