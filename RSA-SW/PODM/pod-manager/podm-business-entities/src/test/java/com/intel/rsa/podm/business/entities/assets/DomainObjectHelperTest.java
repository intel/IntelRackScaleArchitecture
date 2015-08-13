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

package com.intel.rsa.podm.business.entities.assets;

import com.intel.rsa.common.types.Id;
import com.intel.rsa.podm.business.entities.DomainObjectNotFoundException;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import org.testng.annotations.Test;

import java.util.Collection;
import java.util.LinkedList;

import static com.intel.rsa.common.types.Id.id;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.testng.AssertJUnit.assertEquals;

public class DomainObjectHelperTest {

    @Test
    public void positiveTestFindById() throws Exception {
        Collection<DomainObject> testSet = createDomainObjectCollectionWithId(id(1), id(2), id(3));

        DomainObject result = DomainObjectHelper.findById(id(2), testSet);

        assertEquals(result.getId(), id(2));
    }

    @Test (expectedExceptions = DomainObjectNotFoundException.class)
    public void negativeTestFindById() throws Exception {
        Collection<DomainObject> testSet = createDomainObjectCollectionWithId(id(1), id(2), id(3));

        DomainObject result = DomainObjectHelper.findById(id(4), testSet);

        assertEquals(result.getId(), id(4));
    }

    private Collection<DomainObject> createDomainObjectCollectionWithId(Id ... ids) {
        Collection<DomainObject> domainObjects = new LinkedList<>();
        for (Id id : ids) {
            domainObjects.add(createDomainObjectWithId(id));
        }
        return domainObjects;
    }

    private DomainObject createDomainObjectWithId(Id id) {
        DomainObject object = mock(DomainObject.class);
        when(object.getId()).thenReturn(id);
        return object;
    }
}
