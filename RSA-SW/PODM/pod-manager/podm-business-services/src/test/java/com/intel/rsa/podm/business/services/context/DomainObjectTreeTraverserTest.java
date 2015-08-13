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
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import org.testng.annotations.Test;

import static com.intel.rsa.common.types.Id.id;
import static com.intel.rsa.podm.business.services.context.Context.contextOf;
import static com.intel.rsa.podm.business.services.context.ContextType.BLADE;
import static com.intel.rsa.podm.business.services.context.ContextType.COMPUTE_MODULE;
import static com.intel.rsa.podm.business.services.context.ContextType.DRAWER;
import static com.intel.rsa.podm.business.services.context.ContextType.POD;
import static com.intel.rsa.podm.business.services.context.ContextType.RACK;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertEquals;

public class DomainObjectTreeTraverserTest {
    @Test
    public void whenTraversingPodContext_shouldReturnDomainObjectFromRepository() throws EntityNotFoundException {
        DomainObjectTreeTraverser sut = createDomainObjectTreeTraverser();

        when(sut.validator.isValid(any())).thenReturn(true);

        when(sut.mapper.get(any())).thenReturn((Class) Pod.class);

        Pod expectedPod = mock(Pod.class);
        when(sut.repository.get(any(), any())).thenReturn(expectedPod);

        Context podContext = contextOf(id(1), POD);
        DomainObject actualDomainObject = sut.traverse(podContext);

        assertEquals(actualDomainObject, expectedPod);
    }

    @Test(expectedExceptions = EntityNotFoundException.class)
    public void whenTraversingInvalidPodContext_shouldThrow() throws EntityNotFoundException {
        DomainObjectTreeTraverser sut = createDomainObjectTreeTraverser();

        Context podContext = contextOf(id(1), POD);
        when(sut.validator.isValid(podContext)).thenReturn(false);

        sut.traverse(podContext);
    }

    @Test
    public void whenTraversingBladeContext_shouldUseContextTypeToDomainObjectClassMapperWithRepositoryTogether() throws EntityNotFoundException {
        DomainObjectTreeTraverser sut = createDomainObjectTreeTraverser();
        when(sut.validator.isValid(any())).thenReturn(true);

        MockedBlade mockedBlade = mock(MockedBlade.class);

        when(sut.mapper.get(BLADE)).thenReturn((Class) MockedBlade.class);

        Context bladeContext = contextOf(id(2), POD)
                .child(id(3), RACK)
                .child(id(4), DRAWER)
                .child(id(5), COMPUTE_MODULE)
                .child(id(6), BLADE);

        sut.traverse(bladeContext);

        verify(sut.repository).get(eq(MockedBlade.class), any());
    }

    @Test
    public void whenTraversingDifferentContexts_shouldUseContextTypeToDomainObjectClassMapperWithCorrectParameters() throws EntityNotFoundException {
        DomainObjectTreeTraverser sut = createDomainObjectTreeTraverser();
        when(sut.validator.isValid(any())).thenReturn(true);

        MockedRack mockedRack = mock(MockedRack.class);
        MockedBlade mockedBlade = mock(MockedBlade.class);

        when(sut.mapper.get(RACK)).thenReturn((Class) MockedRack.class);
        when(sut.mapper.get(BLADE)).thenReturn((Class) MockedBlade.class);

        Context rackContext = contextOf(id(20), POD)
                .child(id(30), RACK);

        Context bladeContext = contextOf(id(22), POD)
                .child(id(33), RACK)
                .child(id(44), DRAWER)
                .child(id(55), COMPUTE_MODULE)
                .child(id(66), BLADE);

        sut.traverse(rackContext);
        sut.traverse(bladeContext);

        verify(sut.mapper).get(RACK);
        verify(sut.mapper).get(BLADE);
    }

    @Test
    public void whenTraversingRackContext_ShouldUseRepositoryWithCorrectParameters() throws EntityNotFoundException {
        DomainObjectTreeTraverser sut = createDomainObjectTreeTraverser();

        when(sut.validator.isValid(any())).thenReturn(true);
        when(sut.mapper.get(any())).thenReturn((Class) MockedRack.class);

        Context podContext = contextOf(id(10), POD).child(id(20), RACK);

        sut.traverse(podContext);

        verify(sut.repository).get(MockedRack.class, id(20));
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenTraversingWithNullContext_shouldThrow() throws EntityNotFoundException {
        DomainObjectTreeTraverser sut = createDomainObjectTreeTraverser();
        sut.traverse(null);
    }

    private DomainObjectTreeTraverser createDomainObjectTreeTraverser() {
        DomainObjectTreeTraverser sut = new DomainObjectTreeTraverser();
        sut.validator = mock(ContextValidator.class);
        sut.repository = mock(DomainObjectRepository.class);
        sut.mapper = mock(ContextTypeToDomainObjectMapper.class);
        return sut;
    }

    public static class MockedBlade extends DomainObject {
    }

    public static class MockedRack extends DomainObject {
    }
}
