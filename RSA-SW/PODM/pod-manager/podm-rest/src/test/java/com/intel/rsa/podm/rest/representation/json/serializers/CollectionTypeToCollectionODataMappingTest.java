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

package com.intel.rsa.podm.rest.representation.json.serializers;

import com.google.common.collect.Sets;
import com.intel.rsa.podm.business.dto.CollectionDtoType;
import org.testng.annotations.Test;

import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

import static java.util.Arrays.asList;
import static org.testng.Assert.assertEquals;

public class CollectionTypeToCollectionODataMappingTest {

    @Test
    public void checkWhetherAllCollectionDtoTypesAreMapped() throws Exception {
        Set<CollectionDtoType> mappedCollectionTypes = CollectionTypeToCollectionODataMapping.COLLECTION_TYPE_TO_ODATA_MAPPING.keySet();
        Set<CollectionDtoType> allCollectionDtoTypes = new HashSet<>(asList(CollectionDtoType.values()));

        assertEquals(Sets.symmetricDifference(mappedCollectionTypes, allCollectionDtoTypes), Collections.emptySet());
    }
}
