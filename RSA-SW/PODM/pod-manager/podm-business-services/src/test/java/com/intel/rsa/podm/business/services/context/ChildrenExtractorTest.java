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

import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.Rack;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import java.util.ArrayList;
import java.util.Collection;

import static java.util.Collections.singletonList;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertEquals;
import static org.testng.Assert.fail;

public class ChildrenExtractorTest {
    private static final ContextTypeToDomainObjectMapper CONTEXT_TYPE_TO_DOMAIN_OBJECT_MAPPER = new ContextTypeToDomainObjectMapper();

    @Test(dataProvider = "allAssociations")
    public void checkWhetherExtractorExists(Class parentClass, Class childClass) {
        for (ChildrenExtractor extractor : ChildrenExtractor.values()) {
            if (extractor.getParentClass() == parentClass &&  extractor.getChildClass() == childClass) {
                return;
            }

        }

        fail("extractor has not been found");
    }

    @DataProvider
    public Object[][] allAssociations() {
        ArrayList<Object[]> result = new ArrayList<>();

        for (ContextType contextType : ContextType.values()) {
            Class childClass = CONTEXT_TYPE_TO_DOMAIN_OBJECT_MAPPER.get(contextType);

            for (ContextType parentType : contextType.getPossibleParents()) {
                Class parentClass = CONTEXT_TYPE_TO_DOMAIN_OBJECT_MAPPER.get(parentType);

                result.add(new Object[] {parentClass, childClass});
            }

        }

        return result.toArray(new Object[result.size()][]);
    }

    @Test
    public void whenGettingChildrenOfPod_shouldReturnCollectionOfExpectedRack() {
        Pod pod = mock(Pod.class);
        Rack rack = mock(Rack.class);

        Collection<Rack> expected = singletonList(rack);

        when(pod.getRacks()).thenReturn(expected);

        Collection<DomainObject> actual =  ChildrenExtractor.RACK_FROM_POD.extractFrom(pod);

        assertEquals(actual, expected);
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenGettingChildrenOfPodFromInvalidExtractor_shouldThrow() {
        Pod pod = mock(Pod.class);

        ChildrenExtractor.COMPUTE_MODULE_FROM_DRAWER.extractFrom(pod);
    }
}
