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
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.UUID;

import static java.lang.String.format;
import static javax.transaction.Transactional.TxType.MANDATORY;

@Dependent
@Transactional(MANDATORY)
public class ExternalServiceRepository {
    @Inject
    private DomainObjectRepository repository;

    public ExternalService find(UUID uuid) {
        ExternalService service = findOrNull(uuid);

        if (service == null) {
            String msg = format("there is no service with UUID '%s'", uuid);
            throw new IllegalStateException(msg);
        }

        return service;
    }

    public ExternalService findOrNull(UUID uuid) {
        if (uuid == null) {
            throw new IllegalArgumentException("uuid must not be null");
        }

        return repository.getSingleByProperty(ExternalService.class, ExternalService.UUID, uuid);
    }

    public ExternalService create(UUID uuid, ExternalService.Type serviceType) {
        if (uuid == null) {
            throw new IllegalArgumentException("uuid must not be null");
        }

        if (findOrNull(uuid) != null) {
            String msg = String.format("service with UUID '%s' exists", uuid);
            throw new IllegalStateException(msg);
        }

        ExternalService service = repository.create(ExternalService.class);
        service.setUuid(uuid);
        service.setServiceType(serviceType);
        return service;
    }
}
