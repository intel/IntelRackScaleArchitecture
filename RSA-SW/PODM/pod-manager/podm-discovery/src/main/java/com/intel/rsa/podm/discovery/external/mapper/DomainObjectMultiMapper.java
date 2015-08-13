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

package com.intel.rsa.podm.discovery.external.mapper;

import com.intel.rsa.client.api.reader.RsaResource;
import com.intel.rsa.client.api.reader.RsaServiceRoot;
import com.intel.rsa.podm.business.entities.assets.ExternalService;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;

import javax.enterprise.context.Dependent;
import javax.enterprise.inject.Instance;
import javax.inject.Inject;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import static java.lang.String.format;

@Dependent
public class DomainObjectMultiMapper {
    @Inject
    private Instance<DomainObjectMapper<? extends RsaResource, ? extends DomainObject>> mapperPool;

    @Inject
    private DomainObjectRepository repository;

    public Map<RsaResource, DomainObject> map(Collection<RsaResource> resources, ExternalService externalService) {
        if (externalService == null) {
            throw new NullPointerException("externalService should not be null");
        }

        Map<RsaResource, DomainObject> result = new HashMap<>();

        for (RsaResource resource : resources) {
            DomainObject domainObject = map(resource, externalService);
            result.put(resource, domainObject);
        }

        return result;
    }

    private DomainObject map(RsaResource resource, ExternalService service) {
        if (resource instanceof RsaServiceRoot) {
            return null;
        }

        Class<? extends RsaResource> resourceClass = resource.getClass();

        for (DomainObjectMapper mapper : mapperPool) {
            if (mapper.getSourceClass().isAssignableFrom(resourceClass)) {
                Class<DomainObject> targetClass = mapper.getTargetClass();

                DomainObject target = service.findOrCreate(resource.getUri(), (Class) targetClass);
                mapper.map(resource, target);
                return target;
            }
        }

        String msg = format("There is no mapper for source class: %s", resourceClass);
        throw new UnsupportedOperationException(msg);
    }
}
