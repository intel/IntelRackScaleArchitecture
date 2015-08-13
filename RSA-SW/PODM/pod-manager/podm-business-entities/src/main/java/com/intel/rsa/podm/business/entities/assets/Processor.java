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

package com.intel.rsa.podm.business.entities.assets;

import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.InstructionSet;
import com.intel.rsa.common.types.InstructionSetExtension;
import com.intel.rsa.common.types.helpers.ConvertableEnumListHolder;
import com.intel.rsa.common.types.ProcessorModel;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.assets.properties.ProcessorId;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.common.utils.IterableHelper.singleOrNull;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.convertableEnumListProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class Processor extends Asset {

    public static final DomainObjectProperty<String> SOCKET = stringProperty("socket");
    public static final DomainObjectProperty<ProcessorModel> MODEL = enumProperty("model", ProcessorModel.class);
    public static final DomainObjectProperty<String> MANUFACTURER = stringProperty("manufacturer");

    public static final DomainObjectProperty<String> PROCESSOR_TYPE = stringProperty("processorType");
    public static final DomainObjectProperty<String> PROCESSOR_ARCHITECTURE = stringProperty("processorArchitecture");
    public static final DomainObjectProperty<InstructionSet> INSTRUCTION_SET = enumProperty("instructionSet", InstructionSet.class);
    public static final DomainObjectProperty<ConvertableEnumListHolder<InstructionSetExtension>> INSTRUCTION_SET_EXTENSIONS =
            convertableEnumListProperty("instructionSetExtensions", InstructionSetExtension.class);

    public static final DomainObjectProperty<Integer> MAX_SPEED_MHZ = integerProperty("maxSpeedMHz");
    public static final DomainObjectProperty<Integer> TOTAL_CORES = integerProperty("totalCores");
    public static final DomainObjectProperty<Integer> ENABLED_CORES = integerProperty("enabledCores");
    public static final DomainObjectProperty<Integer> TOTAL_THREADS = integerProperty("totalThreads");
    public static final DomainObjectProperty<Integer> ENABLED_THREADS = integerProperty("enabledThreads");

    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("healthRollup", Health.class);

    public Blade getBlade() {
        try {
            return single(getLinked(AssetLink.CONTAINED_BY, Blade.class));
        } catch (IllegalStateException e) {
            throw new IllegalStateException("Processor has to be contained by exactly one Blade", e);
        }
    }

    @Override
    public Location getLocation() {
        return getBlade().getLocation();
    }

    public void setProcessorId(ProcessorId processorId) {
        link(AssetLink.CONTAINS, processorId);
    }

    public ProcessorId getProcessorId() {
        return singleOrNull(getLinked(AssetLink.CONTAINS, ProcessorId.class));
    }

    public String getSocket() {
        return getProperty(SOCKET);
    }

    public void setSocket(String socket) {
        setProperty(SOCKET, socket);
    }

    public ProcessorModel getModel() {
        return getProperty(MODEL);
    }

    public void setModel(ProcessorModel model) {
        setProperty(MODEL, model);
    }

    public String getManufacturer() {
        return getProperty(MANUFACTURER);
    }

    public void setManufacturer(String manufacturer) {
        setProperty(MANUFACTURER, manufacturer);
    }

    public String getProcessorType() {
        return getProperty(PROCESSOR_TYPE);
    }

    public void setProcessorType(String processorType) {
        setProperty(PROCESSOR_TYPE, processorType);
    }

    public String getProcessorArchitecture() {
        return getProperty(PROCESSOR_ARCHITECTURE);
    }

    public void setProcessorArchitecture(String processorArchitecture) {
        setProperty(PROCESSOR_ARCHITECTURE, processorArchitecture);
    }

    public ConvertableEnumListHolder<InstructionSetExtension> getInstructionSetExtensions() {
        return getProperty(INSTRUCTION_SET_EXTENSIONS);
    }

    public void setInstructionSetExtensions(ConvertableEnumListHolder<InstructionSetExtension> convertableEnumListHolder) {
        setProperty(INSTRUCTION_SET_EXTENSIONS, convertableEnumListHolder);
    }

    public InstructionSet getInstructionSet() {
        return getProperty(INSTRUCTION_SET);
    }

    public void setInstructionSet(InstructionSet instructionSet) {
        setProperty(INSTRUCTION_SET, instructionSet);
    }

    public Integer getMaxSpeedMhz() {
        return getProperty(MAX_SPEED_MHZ);
    }

    public void setMaxSpeedMhz(Integer maxSpeedMhz) {
        setProperty(MAX_SPEED_MHZ, maxSpeedMhz);
    }

    public Integer getTotalCores() {
        return getProperty(TOTAL_CORES);
    }

    public void setTotalCores(Integer totalCores) {
        setProperty(TOTAL_CORES, totalCores);
    }

    public Integer getEnabledCores() {
        return getProperty(ENABLED_CORES);
    }

    public void setEnabledCores(Integer enabledCores) {
        setProperty(ENABLED_CORES, enabledCores);
    }

    public Integer getTotalThreads() {
        return getProperty(TOTAL_THREADS);
    }

    public void setTotalThreads(Integer totalCores) {
        setProperty(TOTAL_THREADS, totalCores);
    }

    public Integer getEnabledThreads() {
        return getProperty(ENABLED_THREADS);
    }

    public void setEnabledThreads(Integer enabledThreads) {
        setProperty(ENABLED_THREADS, enabledThreads);
    }

    public State getState() {
        return getProperty(STATE);
    }

    public void setState(State state) {
        setProperty(STATE, state);
    }

    public Health getHealth() {
        return getProperty(HEALTH);
    }

    public void setHealth(Health health) {
        setProperty(HEALTH, health);
    }

    public Health getHealthRollup() {
        return getProperty(HEALTH_ROLLUP);
    }

    public void setHealthRollup(Health healthRollup) {
        setProperty(HEALTH_ROLLUP, healthRollup);
    }
}
