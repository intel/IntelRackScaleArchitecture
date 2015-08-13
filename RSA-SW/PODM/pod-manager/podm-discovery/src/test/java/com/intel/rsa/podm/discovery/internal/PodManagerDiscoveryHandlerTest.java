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

package com.intel.rsa.podm.discovery.internal;

import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.ManagerCollection;
import com.intel.rsa.podm.business.entities.assets.ManagerCollectionProvider;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.properties.Console;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.configuration.VersionLoader;
import org.testng.annotations.Test;

import static com.google.common.collect.ImmutableList.of;
import static java.util.Collections.emptyList;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

public class PodManagerDiscoveryHandlerTest {

    @Test(expectedExceptions = IllegalArgumentException.class, expectedExceptionsMessageRegExp = ".*Pod.*")
    public void whenCreatingManagerForNullPod_ShouldThrow() {
        PodManagerDiscoveryHandler handler = createPodManagerDiscoveryHandler();
        handler.getManagerForPod(null);
    }

    @Test
    public void whenGettingManagerForPod_ShouldNotCreateManagerWhenManagerExists() {
        PodManagerDiscoveryHandler handler = createPodManagerDiscoveryHandler();
        Pod mockedPod = mock(Pod.class);
        when(mockedPod.getManagers()).thenReturn(of(new Manager()));

        handler.getManagerForPod(mockedPod);

        verify(mockedPod).getManagers();
        verify(handler.repository, never()).create(Manager.class);
    }


    @Test
    public void whenCreatingManagerForPod_ShouldLinkItToGlobalManagersCollection() {
        PodManagerDiscoveryHandler handler = createPodManagerDiscoveryHandler();
        Pod mockedPod = mock(Pod.class);
        when(mockedPod.getManagers()).thenReturn(emptyList());

        Manager managerForPod = handler.getManagerForPod(mockedPod);

        verify(handler.managerCollectionProvider.getInstance()).link(managerForPod);
    }

    @Test
    public void whenCreatingManagerForPod_ShouldLinkItToPod() {
        PodManagerDiscoveryHandler handler = createPodManagerDiscoveryHandler();
        Pod mockedPod = mock(Pod.class);
        when(mockedPod.getManagers()).thenReturn(emptyList());

        Manager managerForPod = handler.getManagerForPod(mockedPod);

        verify(mockedPod).linkManager(managerForPod);
    }

    @Test
    public void whenCreatingManagerForPod_ShouldAddNetworkInterfaces() {
        PodManagerDiscoveryHandler handler = createPodManagerDiscoveryHandler();

        Pod mockedPod = mock(Pod.class);
        when(mockedPod.getManagers()).thenReturn(emptyList());

        Manager managerForPod = handler.getManagerForPod(mockedPod);

        verify(handler.podManagerNetworkInterfaceDiscoveryHandler).addNetworkInterfaces(managerForPod);
    }

    @Test
    public void whenCreatingManagerForPod_ShouldAddNetworkService() {
        PodManagerDiscoveryHandler handler = createPodManagerDiscoveryHandler();
        Pod mockedPod = mock(Pod.class);
        when(mockedPod.getManagers()).thenReturn(emptyList());

        Manager managerForPod = handler.getManagerForPod(mockedPod);

        verify(handler.podManagerNetworkServiceDiscoveryHandler).addNetworkService(managerForPod);
    }

    private PodManagerDiscoveryHandler createPodManagerDiscoveryHandler() {
        PodManagerDiscoveryHandler handler = new PodManagerDiscoveryHandler();
        handler.repository = mock(DomainObjectRepository.class);

        Manager manager = mock(Manager.class);
        when(manager.getGraphicalConsole()).thenReturn(mock(Console.class));
        when(manager.getSerialConsole()).thenReturn(mock(Console.class));
        when(manager.getCommandShell()).thenReturn(mock(Console.class));

        when(handler.repository.create(Manager.class)).thenReturn(manager);
        when(handler.repository.create(Console.class)).thenReturn(mock(Console.class));

        handler.managerCollectionProvider = mock(ManagerCollectionProvider.class);
        doReturn(mock(ManagerCollection.class)).when(handler.managerCollectionProvider).getInstance();

        handler.podManagerNetworkInterfaceDiscoveryHandler = mock(PodManagerNetworkInterfaceDiscoveryHandler.class);
        handler.podManagerNetworkServiceDiscoveryHandler = mock(PodManagerNetworkServiceDiscoveryHandler.class);
        handler.versionLoader = mock(VersionLoader.class);

        return handler;
    }
}
