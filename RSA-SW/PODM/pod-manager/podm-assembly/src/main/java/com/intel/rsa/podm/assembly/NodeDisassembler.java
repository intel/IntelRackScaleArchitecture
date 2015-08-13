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

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.podm.actions.ActionException;
import com.intel.rsa.podm.actions.ResetActionInvoker;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.business.entities.components.ComposedNode;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.common.types.ComposedNodeState.POWERED_ON;

@RequestScoped
public class NodeDisassembler {

    @Inject
    private Logger logger;

    @Inject
    private DomainObjectRepository repository;

    @Inject
    private ResetActionInvoker actionInvoker;

    @Transactional(value = Transactional.TxType.MANDATORY, rollbackOn = DisassemblyException.class)
    public void disassemble(ComposedNode composedNode) throws DisassemblyException {
        Blade blade = composedNode.getBlade();

        if (composedNode.getComposedNodeState().equals(POWERED_ON)) {
            try {
                actionInvoker.shutdownGracefully(blade);
            } catch (ActionException e) {
                throw new DisassemblyException("Could not power off composed node.");
            }
        }

        blade.setAllocated(false);
        for (RemoteTarget target : composedNode.getRemoteTargets()) {
            target.setAllocated(false);
        }
    }
}
