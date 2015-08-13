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

package com.intel.rsa.podm.assembly;

import com.intel.rsa.common.types.BootSourceType;
import com.intel.rsa.podm.actions.ActionException;
import com.intel.rsa.podm.actions.ResetActionInvoker;
import com.intel.rsa.podm.actions.BootSourceOverrideInvoker;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.components.ComposedNode;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.common.types.BootSourceType.HDD;
import static com.intel.rsa.common.types.BootSourceType.PXE;
import static com.intel.rsa.common.types.ComposedNodeState.ALLOCATED;
import static com.intel.rsa.common.types.ComposedNodeState.FAILED;
import static com.intel.rsa.common.types.ComposedNodeState.POWERED_ON;

@RequestScoped
public class NodeAssembler {

    @Inject
    private ResetActionInvoker resetActionInvoker;

    @Inject
    private BootSourceOverrideInvoker bootSourceOverrideInvoker;

    @Transactional(Transactional.TxType.REQUIRED)
    public void assemble(ComposedNode composedNode) throws AssemblyException {
        if (!ALLOCATED.equals(composedNode.getComposedNodeState())) {
            throw new AssemblyException(String.format("Only composed node in %s state can be assembled", ALLOCATED));
        }

        Blade blade = getComposedNodeBlade(composedNode);
        BootSourceType bootSourceType = composedNode.getRemoteTargets().size() > 0 ? PXE : HDD;
        try {
            resetActionInvoker.powerOff(blade);
            bootSourceOverrideInvoker.overrideBootSourcePermanently(blade, bootSourceType);
            resetActionInvoker.powerOn(blade);
        } catch (ActionException e) {
            composedNode.setComposedNodeState(FAILED);
            throw new AssemblyException("Assembly failed on Blade action", e);
        }

        composedNode.setComposedNodeState(POWERED_ON);
    }

    private Blade getComposedNodeBlade(ComposedNode composedNode) throws AssemblyException {
        Blade blade = composedNode.getBlade();
        if (blade == null) {
            throw new AssemblyException("No Blade is associated with Composed Node. Assembly aborted");
        }
        return blade;
    }
}
