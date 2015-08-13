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

package com.intel.rsa.podm.rest.odataid;

import com.intel.rsa.podm.business.services.context.ContextType;
import org.testng.annotations.Test;

import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

import static com.google.common.collect.Sets.difference;
import static com.google.common.collect.Sets.newHashSet;
import static com.google.common.collect.Sets.symmetricDifference;
import static com.intel.rsa.podm.rest.resources.ResourceNames.ALL_RESOURCE_NAMES;
import static com.intel.rsa.podm.rest.resources.ResourceNames.NETWORK_SERVICE_RESOURCE_NAME;
import static java.util.Arrays.asList;
import static org.testng.Assert.assertEquals;

public class ContextTypeToResourceNameMapperTest {

    @Test
    public void whenGettingResourceNameForPodContext_shouldReturnPodsString() {
        ContextTypeToResourceNameMapper sut = new ContextTypeToResourceNameMapper();

        assertEquals(sut.get(ContextType.POD), "Pods");
    }

    @Test
    public void whenGettingResourceNameForDriveContext_shouldReturnDrivesString() {
        ContextTypeToResourceNameMapper sut = new ContextTypeToResourceNameMapper();

        assertEquals(sut.get(ContextType.DRIVE), "Drives");
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenGettingMappedResourceNameForNullContextType_shouldThrow() {
        ContextTypeToResourceNameMapper sut = new ContextTypeToResourceNameMapper();
        sut.get(null);
    }

    @Test
    public void checkWhetherAllContextTypesAreMapped() {
        Set<ContextType> mappedContextTypes = ContextTypeToResourceNameMapper.MAPPING.keySet();
        Set<ContextType> allContextTypes = new HashSet<>(asList(ContextType.values()));

        assertEquals(symmetricDifference(mappedContextTypes, allContextTypes), Collections.emptySet());
    }

    @Test(enabled = false)
    public void checkWhetherMappingIsInjective() {
        Collection<String> values = ContextTypeToResourceNameMapper.MAPPING.values();

        int numberOfMappedValues = values.size();
        int numberOfDistinctValues = new HashSet<>(values).size();
        //todo handling multiple types with same resource name e.g Drive
        assertEquals(numberOfMappedValues, numberOfDistinctValues);
    }

    @Test
    public void checkWhetherAllCollectionResourceNameAreMapped() {
        Set<String> mappedResourceNames = newHashSet(ContextTypeToResourceNameMapper.MAPPING.values());
        Set<String> allCollectionResourceNames = difference(ALL_RESOURCE_NAMES, newHashSet(NETWORK_SERVICE_RESOURCE_NAME));

        assertEquals(symmetricDifference(mappedResourceNames, allCollectionResourceNames), Collections.emptySet());
    }
}
