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

import com.intel.rsa.common.types.ConnectionType;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.ManagerType;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.dto.ConsoleDto;
import com.intel.rsa.podm.business.dto.ManagerDto;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.ManagerJson;
import org.testng.annotations.BeforeTest;
import org.testng.annotations.Test;

import java.net.URI;
import java.time.OffsetDateTime;
import java.util.Arrays;
import java.util.Collection;
import java.util.LinkedList;
import java.util.List;
import java.util.function.Function;

import static com.intel.rsa.common.types.Id.id;
import static com.intel.rsa.podm.business.services.context.Context.contextOf;
import static com.intel.rsa.podm.business.services.context.ContextType.POD;
import static com.intel.rsa.podm.business.services.context.ContextType.RACK;
import static com.intel.rsa.podm.business.services.context.ContextType.DRAWER;
import static com.intel.rsa.podm.business.services.context.ContextType.COMPUTE_MODULE;
import static com.intel.rsa.podm.business.services.context.ContextType.FABRIC_MODULE;
import static com.intel.rsa.podm.business.services.context.ContextType.BLADE;
import static com.intel.rsa.podm.business.services.context.ContextType.SWITCH;
import static org.testng.Assert.assertEquals;

public class ManagerDtoJsonSerializerTest {
    private static final Id ID = Id.fromString("1");
    private static final String NAME = "Drawer Manager";
    private static final ManagerType MANAGER_TYPE = ManagerType.ENCLOSURE_MANAGER;
    private static final String MODEL = "Avoton CPP";
    private static final State STATE = State.ENABLED;
    private static final Health HEALTH = Health.OK;
    private static final Health HEALTH_ROLLUP = Health.OK;
    private static final String FIRMWARE_VERSION = "1.1.2.24.0 (debug build Feb 16 2015 16:46:33)";

    private static final boolean EMPTY_CONSOLE_ENABLED = false;
    private static final Integer EMPTY_CONSOLE_MAX_CONCURRENT_SEESIONS = null;
    private static final ConnectionType EMPTY_CONSOLE_CONNECT_TYPE_SUPPORTED = null;

    private static final List<Context> MANAGER_FOR_DRAWERS = Arrays.asList(
            contextOf(id(1), POD).child(id(1), RACK).child(id(1), DRAWER),
            contextOf(id(1), POD).child(id(1), RACK).child(id(2), DRAWER));
    private static final List<Context> MANAGER_FOR_COMPUTE_MODULES = Arrays.asList(
            contextOf(id(1), POD).child(id(1), RACK).child(id(1), DRAWER).child(id(1), COMPUTE_MODULE),
            contextOf(id(1), POD).child(id(1), RACK).child(id(1), DRAWER).child(id(2), COMPUTE_MODULE));
    private static final List<Context> MANAGER_FOR_FABRIC_MODULES = Arrays.asList(
            contextOf(id(1), POD).child(id(1), RACK).child(id(1), DRAWER).child(id(1), FABRIC_MODULE),
            contextOf(id(1), POD).child(id(1), RACK).child(id(1), DRAWER).child(id(2), FABRIC_MODULE));
    private static final List<Context> MANAGER_FOR_BLADES = Arrays.asList(
            contextOf(id(1), POD).child(id(1), RACK).child(id(1), DRAWER).child(id(1), COMPUTE_MODULE).child(id(1), BLADE),
            contextOf(id(1), POD).child(id(1), RACK).child(id(1), DRAWER).child(id(2), COMPUTE_MODULE).child(id(2), BLADE));
    private static final List<Context> MANAGER_FOR_SWITCHES = Arrays.asList(
            contextOf(id(1), POD).child(id(1), RACK).child(id(1), DRAWER).child(id(1), FABRIC_MODULE).child(id(1), SWITCH),
            contextOf(id(1), POD).child(id(1), RACK).child(id(1), DRAWER).child(id(2), FABRIC_MODULE).child(id(2), SWITCH));

    private ManagerDto dto;

    @BeforeTest
    public void createDto() {
        ManagerDto.Builder builder = ManagerDto.newBuilder();
        dto = builder
                .id(ID)
                .currentFirmwareVersion(FIRMWARE_VERSION)
                .graphicalConsole(createEmptyConsoleDto())
                .serialConsole(createEmptyConsoleDto())
                .commandShell(createEmptyConsoleDto())
                .name(NAME)
                .managerType(MANAGER_TYPE)
                .model(MODEL)
                .state(STATE)
                .health(HEALTH)
                .healthRollup(HEALTH_ROLLUP)
                .modified(OffsetDateTime.now())
                .managerForBlades(MANAGER_FOR_BLADES)
                .managerForComputeModules(MANAGER_FOR_COMPUTE_MODULES)
                .managerForFabricModules(MANAGER_FOR_FABRIC_MODULES)
                .managerForDrawers(MANAGER_FOR_DRAWERS)
                .managerForSwitches(MANAGER_FOR_SWITCHES)
                .build();
    }

    @Test
    public void translationTest() {
        ManagerJson managerJson = provideSut().translate(dto);

        assertEquals(managerJson.id, ID);
        assertEquals(managerJson.name, NAME);
        assertEquals(managerJson.managerType, MANAGER_TYPE);
        assertEquals(managerJson.model, MODEL);
        assertEquals(managerJson.status.state, STATE);
        assertEquals(managerJson.status.health, HEALTH);
        assertEquals(managerJson.status.healthRollup, HEALTH_ROLLUP);
        verifyConsole(managerJson.commandShell);
        verifyConsole(managerJson.serialConsole);
        verifyConsole(managerJson.graphicalConsole);
        assertEquals(managerJson.firmware.current.versionString, FIRMWARE_VERSION);
        verifyLinks(managerJson);
    }

    private ManagerDtoJsonSerializer provideSut() {
        ManagerDtoJsonSerializer sut = new ManagerDtoJsonSerializer();
        sut.setContext(() -> URI.create(""));
        return sut;
    }

    private void verifyConsole(ManagerJson.Console console) {
        assertEquals(console.enabled, EMPTY_CONSOLE_ENABLED);
        assertEquals(console.maxConcurrentSessions, EMPTY_CONSOLE_MAX_CONCURRENT_SEESIONS);
        assertEquals(console.connectTypesSupported, EMPTY_CONSOLE_CONNECT_TYPE_SUPPORTED);
    }

    private void verifyLinks(ManagerJson managerJson) {
        assertEquals(managerJson.links.managerForDrawers,
                convertToOdataIds(MANAGER_FOR_DRAWERS, ODataIds::oDataIdFromContext));
        assertEquals(managerJson.links.managerForComputeModules,
                convertToOdataIds(MANAGER_FOR_COMPUTE_MODULES, ODataIds::oDataIdFromContext));
        assertEquals(managerJson.links.managerForFabricModules,
                convertToOdataIds(MANAGER_FOR_FABRIC_MODULES, ODataIds::oDataIdFromContext));
        assertEquals(managerJson.links.managerForBlades,
                convertToOdataIds(MANAGER_FOR_BLADES, ODataIds::oDataIdFromContext));
        assertEquals(managerJson.links.managerForSwitches,
                convertToOdataIds(MANAGER_FOR_SWITCHES, ODataIds::oDataIdFromContext));
    }

    private <E> Collection<ODataId> convertToOdataIds(List<E> ids, Function<E, ODataId> converter) {
        Collection<ODataId> converted = new LinkedList<>();
        ids.forEach(id -> converted.add(converter.apply(id)));
        return converted;
    }

    private ConsoleDto createEmptyConsoleDto() {
        return ConsoleDto
                .newBuilder()
                .enabled(EMPTY_CONSOLE_ENABLED)
                .maxConcurrentSessions(EMPTY_CONSOLE_MAX_CONCURRENT_SEESIONS)
                .connectTypesSupported(EMPTY_CONSOLE_CONNECT_TYPE_SUPPORTED)
                .build();
    }
}
