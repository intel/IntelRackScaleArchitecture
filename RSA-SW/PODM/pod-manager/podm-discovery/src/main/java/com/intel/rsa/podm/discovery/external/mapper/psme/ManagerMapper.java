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

package com.intel.rsa.podm.discovery.external.mapper.psme;

import com.intel.rsa.client.api.reader.psme.PsmeConsole;
import com.intel.rsa.client.api.reader.psme.PsmeManager;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.properties.Console;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.discovery.external.mapper.DomainObjectMapper;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;

@Dependent
public class ManagerMapper extends DomainObjectMapper<PsmeManager, Manager> {

    @Inject
    private DomainObjectRepository repository;

    public ManagerMapper() {
        super(PsmeManager.class, Manager.class);
        registerProvider(Console.class, this::provideConsole);
    }

    private Console provideConsole(PsmeConsole psmeConsole) {
        Console console;
        if (psmeConsole.equals(source.getCommandShell())) {
            console = target.getCommandShell();
        } else if (psmeConsole.equals(source.getGraphicalConsole())) {
            console = target.getGraphicalConsole();
        } else if (psmeConsole.equals(source.getSerialConsole())) {
            console = target.getSerialConsole();
        } else {
            throw new RuntimeException("Unknown console: " + psmeConsole);
        }

        if (console == null) {
            console = repository.create(Console.class);
        }

        return console;
    }

    @Override
    protected void performNotAutomatedMapping(PsmeManager source, Manager target) {
        super.performNotAutomatedMapping(source, target);
        target.updateModified();
    }
}
