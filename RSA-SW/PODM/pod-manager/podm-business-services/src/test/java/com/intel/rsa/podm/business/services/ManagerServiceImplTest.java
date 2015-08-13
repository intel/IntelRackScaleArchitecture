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

package com.intel.rsa.podm.business.services;

import com.intel.rsa.common.types.ConnectionType;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.ManagerType;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.dto.ConsoleDto;
import com.intel.rsa.podm.business.dto.ManagerDto;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.properties.Console;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;
import org.testng.annotations.Test;

import java.time.OffsetDateTime;

import static com.intel.rsa.podm.business.services.context.Context.contextOf;
import static com.intel.rsa.podm.business.services.context.ContextType.MANAGER;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertEquals;
import static org.testng.Assert.assertNotNull;

public class ManagerServiceImplTest {
    public static final ManagerType TYPE = ManagerType.BMC;
    public static final Health HEALTH = Health.OK;
    public static final State STATE = State.ENABLED;
    public static final String FIRMWARE_VERSION = "1";
    public static final String MODEL = "model";
    public static final String NAME = "name";

    private static final boolean CONSOLE_ENABLED = true;
    private static final Integer CONSOLE_MAX_CONCURRENT_SEESIONS = 1;
    private static final ConnectionType CONSOLE_CONNECT_TYPE_SUPPORTED = ConnectionType.OEM;

    @Test
    public void translationTest() throws EntityNotFoundException {
        ManagerServiceImpl service = new ManagerServiceImpl();

        Context managerContext = contextOf(Id.id(1), MANAGER);
        Manager mockedManager = createManagerProvider();
        service.traverser = mock(DomainObjectTreeTraverser.class);
        when(service.traverser.traverse(managerContext)).thenReturn(mockedManager);

        ManagerDto dto = service.getManager(managerContext);

        assertEquals(dto.getName(), NAME);
        assertEquals(dto.getCurrentFirmwareVersion(), FIRMWARE_VERSION);
        assertEquals(dto.getHealth(), HEALTH);
        assertEquals(dto.getManagerType(), TYPE);
        assertEquals(dto.getModel(), MODEL);
        assertNotNull(dto.getModified());
        assertEquals(dto.getState(), STATE);

        checkConsole(dto.getGraphicalConsole());
        checkConsole(dto.getSerialConsole());
        checkConsole(dto.getCommandShell());
    }

    private void checkConsole(ConsoleDto consoleDto) {
        assertEquals(consoleDto.isEnabled(), CONSOLE_ENABLED);
        assertEquals(consoleDto.getMaxConcurrentSessions(), CONSOLE_MAX_CONCURRENT_SEESIONS);
        assertEquals(consoleDto.getConnectTypesSupported(), CONSOLE_CONNECT_TYPE_SUPPORTED);
    }

    private Manager createManagerProvider() {
        Manager manager = mock(Manager.class);
        when(manager.getName()).thenReturn(NAME);
        when(manager.getFirmwareVersion()).thenReturn(FIRMWARE_VERSION);
        when(manager.getHealth()).thenReturn(HEALTH);
        when(manager.getType()).thenReturn(TYPE);
        when(manager.getModel()).thenReturn(MODEL);
        when(manager.getModified()).thenReturn(OffsetDateTime.now());
        when(manager.getState()).thenReturn(STATE);

        Console console = createConsole();
        when(manager.getGraphicalConsole()).thenReturn(console);
        when(manager.getSerialConsole()).thenReturn(console);
        when(manager.getCommandShell()).thenReturn(console);

        return manager;
    }

    private Console createConsole() {
        Console console = mock(Console.class);
        when(console.isEnabled()).thenReturn(CONSOLE_ENABLED);
        when(console.getMaxConcurrentSessions()).thenReturn(CONSOLE_MAX_CONCURRENT_SEESIONS);
        when(console.getConnectTypesSupported()).thenReturn(CONSOLE_CONNECT_TYPE_SUPPORTED);

        return console;
    }

}
