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

import com.intel.rsa.podm.business.entities.assets.Drawer;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.Rack;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import org.testng.annotations.Test;

import static com.intel.rsa.common.types.Id.id;
import static com.intel.rsa.podm.business.services.context.Context.contextOf;
import static com.intel.rsa.podm.business.services.context.ContextType.DRAWER;
import static com.intel.rsa.podm.business.services.context.ContextType.MANAGER;
import static com.intel.rsa.podm.business.services.context.ContextType.POD;
import static com.intel.rsa.podm.business.services.context.ContextType.RACK;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertFalse;
import static org.testng.Assert.assertTrue;

public class ContextValidatorTest {

    /**
     * NUll parent context may be acceptable in some cases by design but it should not be considered as valid context in general.
     * If you need such functionality like checking whether context is valid or null, please provide new method "isValidOrNull".
     */
    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenValidatingNullContext_shouldThrow() {
        ContextValidator sut = createContextValidator();

        sut.isValid(null);
    }

    @Test
    public void whenValidatingContextOfExistingPod_shouldReturnTrue() {
        ContextValidator sut = createContextValidator();

        when(sut.repository.exists(Pod.class, id(2))).thenReturn(true);
        when(sut.repository.get(Pod.class, id(2))).thenReturn(mock(Pod.class));

        Context podContext = contextOf(id(2), POD);

        assertTrue(sut.isValid(podContext));
    }

    @Test
    public void whenValidatingContextOfNonexistentPod_shouldReturnFalse() {
        ContextValidator sut = createContextValidator();

        when(sut.repository.exists(Pod.class, id(2))).thenReturn(false);

        Context podContext = contextOf(id(2), POD);

        assertFalse(sut.isValid(podContext));
    }

    @Test
    public void whenValidatingCorrectContextOfExistingRack_shouldReturnTrue() {
        ContextValidator sut = createContextValidator();

        Pod pod = mock(Pod.class);
        Rack rack = mock(Rack.class);

        when(sut.repository.exists(Pod.class, id(10))).thenReturn(true);
        when(sut.repository.exists(Rack.class, id(20))).thenReturn(true);
        when(sut.repository.get(Pod.class, id(10))).thenReturn(pod);
        when(sut.repository.get(Rack.class, id(20))).thenReturn(rack);

        when(sut.relationshipVerifier.verify(pod, rack)).thenReturn(true);

        Context rackContext = contextOf(id(10), POD).child(id(20), RACK);

        assertTrue(sut.isValid(rackContext));
    }

    @Test
    public void whenValidatingIncorrectContextOfExistingRack_shouldReturnFalse() {
        ContextValidator sut = createContextValidator();

        Pod pod = mock(Pod.class);
        Rack rack = mock(Rack.class);

        when(sut.repository.exists(Pod.class, id(33))).thenReturn(true);
        when(sut.repository.exists(Rack.class, id(66))).thenReturn(true);
        when(sut.repository.get(Pod.class, id(33))).thenReturn(pod);
        when(sut.repository.get(Rack.class, id(66))).thenReturn(rack);

        when(sut.relationshipVerifier.verify(pod, rack)).thenReturn(false);

        Context rackContext = contextOf(id(33), POD).child(id(66), RACK);

        assertFalse(sut.isValid(rackContext));
    }

    @Test
    public void whenValidatingContextOfExistingManager_shouldReturnTrue() {
        ContextValidator sut = createContextValidator();

        when(sut.repository.exists(Manager.class, id(123))).thenReturn(true);
        when(sut.repository.get(Manager.class, id(123))).thenReturn(mock(Manager.class));

        Context managerContext = contextOf(id(123), MANAGER);

        assertTrue(sut.isValid(managerContext));
    }

    @Test
    public void whenValidatingIncorrectContextOfExistingDrawer_shouldReturnFalse() {
        ContextValidator sut = createContextValidator();
        Pod pod = mock(Pod.class);
        Rack rack = mock(Rack.class);
        Drawer drawer = mock(Drawer.class);

        when(sut.repository.exists(Pod.class, id(111))).thenReturn(true);
        when(sut.repository.exists(Rack.class, id(222))).thenReturn(true);
        when(sut.repository.exists(Drawer.class, id(333))).thenReturn(true);
        when(sut.repository.get(Pod.class, id(111))).thenReturn(pod);
        when(sut.repository.get(Rack.class, id(222))).thenReturn(rack);
        when(sut.repository.get(Drawer.class, id(333))).thenReturn(drawer);

        when(sut.relationshipVerifier.verify(pod, rack)).thenReturn(false);
        when(sut.relationshipVerifier.verify(rack, drawer)).thenReturn(true);

        Context drawerContext = contextOf(id(111), POD)
                .child(id(222), RACK)
                .child(id(333), DRAWER);

        assertFalse(sut.isValid(drawerContext));

        verify(sut.relationshipVerifier, never()).verify(eq(null), any());
        verify(sut.relationshipVerifier, never()).verify(any(), eq(null));
    }

    private ContextValidator createContextValidator() {
        ContextValidator sut = new ContextValidator();
        sut.repository = mock(DomainObjectRepository.class);
        sut.relationshipVerifier = mock(ParentChildRelationshipVerifier.class);

        sut.contextTypeToDomainObjectMapper = mock(ContextTypeToDomainObjectMapper.class);
        when(sut.contextTypeToDomainObjectMapper.get(POD)).thenReturn((Class) Pod.class);
        when(sut.contextTypeToDomainObjectMapper.get(RACK)).thenReturn((Class) Rack.class);
        when(sut.contextTypeToDomainObjectMapper.get(DRAWER)).thenReturn((Class) Drawer.class);
        when(sut.contextTypeToDomainObjectMapper.get(MANAGER)).thenReturn((Class) Manager.class);

        return sut;
    }
}
