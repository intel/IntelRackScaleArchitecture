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

import java.util.Collection;

import static java.util.Arrays.stream;
import static java.util.stream.Collectors.toSet;

public enum ContextType {
    POD,
    RACK(POD),
    DRAWER(RACK),
    COMPUTE_MODULE(DRAWER),
    BLADE(COMPUTE_MODULE),
    PROCESSOR(BLADE),
    MEMORY(BLADE),
    STORAGE_CONTROLLER(BLADE),
    DRIVE(STORAGE_CONTROLLER),
    FABRIC_MODULE(DRAWER),
    SWITCH(FABRIC_MODULE),
    SWITCH_PORT(SWITCH),
    VLAN(SWITCH_PORT),
    MANAGER,
    NETWORK_INTERFACE(MANAGER, BLADE),
    COMPOSED_NODE,
    STORAGE_SERVICE,
    PHYSICAL_DRIVE(STORAGE_SERVICE),
    LOGICAL_DRIVE(STORAGE_SERVICE),
    REMOTE_TARGET(STORAGE_SERVICE);

    private final Collection<ContextType> possibleParents;

    ContextType(ContextType... possibleParents) {
        this.possibleParents = stream(possibleParents).collect(toSet());
    }

    Collection<ContextType> getPossibleParents() {
        return possibleParents;
    }
}
