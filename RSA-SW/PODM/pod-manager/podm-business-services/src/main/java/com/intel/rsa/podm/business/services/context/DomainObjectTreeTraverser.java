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

import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static javax.transaction.Transactional.TxType.MANDATORY;


/**
 * Allows to traverse path (expressed by {@link Context}) of {@link DomainObject} tree.
 * Throws exception if {@link DomainObject} is unreachable.
 */
@Transactional(MANDATORY)
public class DomainObjectTreeTraverser {
    @Inject
    DomainObjectRepository repository;

    @Inject
    ContextValidator validator;

    @Inject
    ContextTypeToDomainObjectMapper mapper;

    public DomainObject traverse(Context context) throws EntityNotFoundException {
        if (context == null) {
            throw new IllegalArgumentException("context must not be null");
        }

        if (!validator.isValid(context)) {
            throw new EntityNotFoundException("context is not valid");
        }

        Class<? extends DomainObject> domainObjectClass = mapper.get(context.getType());
        return repository.get(domainObjectClass, context.getId());
    }
}
