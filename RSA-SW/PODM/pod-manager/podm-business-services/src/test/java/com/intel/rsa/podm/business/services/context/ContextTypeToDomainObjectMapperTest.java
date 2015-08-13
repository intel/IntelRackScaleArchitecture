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

import com.google.common.collect.Sets;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import org.testng.annotations.Test;

import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

import static java.util.Arrays.asList;
import static org.testng.Assert.assertEquals;

public class ContextTypeToDomainObjectMapperTest {
    @Test
    public void whenGettingMappedClassForPodContextType_ShouldReturnCorrectClass() {
        ContextTypeToDomainObjectMapper sut = new ContextTypeToDomainObjectMapper();
        assertEquals(sut.get(ContextType.POD), Pod.class);
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenGettingMappedClassForNullContextType_shouldThrow() {
        ContextTypeToDomainObjectMapper sut = new ContextTypeToDomainObjectMapper();
        sut.get(null);
    }

    @Test
    public void checkWhetherAllContextTypesAreMapped() {
        Set<ContextType> mappedContextTypes = ContextTypeToDomainObjectMapper.MAPPING.keySet();

        Set<ContextType> allContextTypes = new HashSet<>(asList(ContextType.values()));

        assertEquals(Sets.symmetricDifference(mappedContextTypes, allContextTypes), Collections.emptySet());
    }

    @Test
    public void checkWhetherMappingIsInjective() {
        Collection<Class<? extends DomainObject>> values = ContextTypeToDomainObjectMapper.MAPPING.values();

        int numberOfMappedValues = values.size();
        int numberOfDistinctValues = new HashSet<>(values).size();

        assertEquals(numberOfMappedValues, numberOfDistinctValues);
    }
}
