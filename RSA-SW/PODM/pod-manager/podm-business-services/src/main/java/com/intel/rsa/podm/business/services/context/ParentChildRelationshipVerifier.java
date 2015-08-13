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

import javax.enterprise.context.Dependent;

/**
 * Allows to determine whether two Domain Objects are associated with parent-child relationship
 */
@Dependent
public class ParentChildRelationshipVerifier {
    public boolean verify(DomainObject parent, DomainObject child) {
        if (parent == null) {
            throw new IllegalArgumentException("parent must not be null");
        }

        if (child == null) {
            throw new IllegalArgumentException("child must not be null");
        }

        for (ChildrenExtractor extractor : ChildrenExtractor.values()) {
            if (extractor.isApplicableFor(parent, child)) {
                return extractor.extractFrom(parent).contains(child);
            }
        }

        return false;
    }
}
