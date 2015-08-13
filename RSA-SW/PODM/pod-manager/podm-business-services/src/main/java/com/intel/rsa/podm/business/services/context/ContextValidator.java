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

package com.intel.rsa.podm.business.services.context;

import com.intel.rsa.common.types.Id;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;

import javax.inject.Inject;

/**
 * Allows to verify whether given {@link ContextType} is correct
 */
public class ContextValidator {
    @Inject
    DomainObjectRepository repository;

    @Inject
    ParentChildRelationshipVerifier relationshipVerifier;

    @Inject
    ContextTypeToDomainObjectMapper contextTypeToDomainObjectMapper;

    public boolean isValid(Context context) {
        if (context == null) {
            throw new IllegalArgumentException("context must not be null");
        }

        DomainObject domainObject = getIfValid(context);
        return domainObject != null;
    }

    private DomainObject getIfValid(Context context) {
        DomainObject domainObject = tryGet(context.getType(), context.getId());

        if (domainObject == null || context.getParent() == null) {
            return domainObject;
        }

        DomainObject parent = getIfValid(context.getParent());

        return parent != null && relationshipVerifier.verify(parent, domainObject)
                ? domainObject
                : null;
    }

    private DomainObject tryGet(ContextType type, Id id) {
        Class domainObjectClass = contextTypeToDomainObjectMapper.get(type);

        return repository.exists(domainObjectClass, id)
                ? repository.get(domainObjectClass, id)
                : null;
    }
}
