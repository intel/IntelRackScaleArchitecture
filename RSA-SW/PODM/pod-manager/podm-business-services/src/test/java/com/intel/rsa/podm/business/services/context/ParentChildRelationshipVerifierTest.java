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

import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.ComputeModule;
import com.intel.rsa.podm.business.entities.assets.Drawer;
import com.intel.rsa.podm.business.entities.assets.Drive;
import com.intel.rsa.podm.business.entities.assets.FabricModule;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.Memory;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.Processor;
import com.intel.rsa.podm.business.entities.assets.Rack;
import com.intel.rsa.podm.business.entities.assets.StorageController;
import com.intel.rsa.podm.business.entities.assets.Switch;
import com.intel.rsa.podm.business.entities.assets.SwitchPort;
import com.intel.rsa.podm.business.entities.assets.VlanNetworkInterface;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import java.util.Collection;
import java.util.function.Function;

import static java.util.Collections.emptyList;
import static java.util.Collections.singletonList;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertFalse;
import static org.testng.Assert.assertTrue;

public class ParentChildRelationshipVerifierTest {
    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenVerifyingNullParent_shouldThrow() {
        ParentChildRelationshipVerifier sut = new ParentChildRelationshipVerifier();
        sut.verify(null, mock(Rack.class));
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenVerifyingNullChild_shouldThrow() {
        ParentChildRelationshipVerifier sut = new ParentChildRelationshipVerifier();
        sut.verify(mock(Rack.class), null);
    }

    @Test(dataProvider = "associatedDomainObjects")
    public void whenCheckingAssociatedDomainObjects_shouldReturnTrue(DomainObject parent, DomainObject child) {
        ParentChildRelationshipVerifier sut = new ParentChildRelationshipVerifier();

        assertTrue(sut.verify(parent, child));
    }

    @Test(dataProvider = "notAssociatedDomainObjects")
    public void whenCheckingNotAssociatedDomainObjects_shouldReturnFalse(DomainObject parent, DomainObject child) {
        ParentChildRelationshipVerifier sut = new ParentChildRelationshipVerifier();

        assertFalse(sut.verify(parent, child));
    }

    @DataProvider
    public Object[][] associatedDomainObjects() {
        return new Object[][] {
                mockAssociated(Pod.class, Rack.class, Pod::getRacks),
                mockAssociated(Rack.class, Drawer.class, Rack::getDrawers),
                mockAssociated(Drawer.class, ComputeModule.class, Drawer::getComputeModules, Drawer::getFabricModules),
                mockAssociated(ComputeModule.class, Blade.class, ComputeModule::getBlades),
                mockAssociated(Blade.class, Processor.class, Blade::getProcessors),
                mockAssociated(Blade.class, Memory.class, Blade::getMemory),
                mockAssociated(Blade.class, StorageController.class, Blade::getStorageControllers),
                mockAssociated(Blade.class, NetworkInterface.class, Blade::getEthernetInterfaces),
                mockAssociated(StorageController.class, Drive.class, StorageController::getDrives),
                mockAssociated(FabricModule.class, Switch.class, FabricModule::getSwitches),
                mockAssociated(Switch.class, SwitchPort.class, Switch::getSwitchPorts),
                mockAssociated(SwitchPort.class, VlanNetworkInterface.class, SwitchPort::getVlans),
                mockAssociated(Manager.class, NetworkInterface.class, Manager::getNetworkInterfaces)
        };
    }

    @DataProvider
    public Object[][] notAssociatedDomainObjects() {
        return new Object[][] {
                mockNotAssociated(Pod.class, Rack.class, Pod::getRacks),
                mockNotAssociated(Rack.class, Drawer.class, Rack::getDrawers),
        };
    }

    private <P extends DomainObject, C extends DomainObject>
    Object[] mockAssociated(Class<P> parentClass,
                            Class<C> childClass,
                            Function<P, Collection<C>> childCollection,
                            Function<P, Collection<? extends DomainObject>>... otherChildCollections) {
        P parent = mock(parentClass, parentClass.getSimpleName());
        C child = mock(childClass, childClass.getSimpleName());

        when(childCollection.apply(parent)).thenReturn(singletonList(child));

        for (Function<P, Collection<? extends DomainObject>> other : otherChildCollections) {
            when(other.apply(parent)).thenReturn(emptyList());
        }

        return new Object[] {parent, child};
    }

    private <P extends DomainObject, C extends DomainObject>
    Object[] mockNotAssociated(Class<P> parentClass, Class<C> childClass, Function<P, Collection<C>> childCollection) {
        P parent = mock(parentClass, parentClass.getSimpleName());
        C notChild = mock(childClass, childClass.getSimpleName());

        when(childCollection.apply(parent)).thenReturn(emptyList());

        return new Object[] {parent, notChild};
    }
}
