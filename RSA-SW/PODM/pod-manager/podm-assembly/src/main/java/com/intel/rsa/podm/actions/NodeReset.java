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

package com.intel.rsa.podm.actions;

import com.intel.rsa.common.types.ComposedNodeState;
import com.intel.rsa.common.types.actions.ResetType;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.components.ComposedNode;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import javax.transaction.Transactional;

import java.util.EnumSet;
import java.util.Set;

import static com.intel.rsa.common.types.ComposedNodeState.POWERED_OFF;
import static com.intel.rsa.common.types.ComposedNodeState.POWERED_ON;

/**
 * Performs reset on node. Delegates actions to node components.
 * TODO only power on/off are supported now. Implement funcionality for other types.
 */
@RequestScoped
public class NodeReset {
    private static final Set<ResetType> SUPPORTED_RESET_TYPES = EnumSet.of(
            ResetType.FORCE_OFF, ResetType.ON, ResetType.SOFT_SHUTDOWN
    );

    @Inject
    private ResetActionInvoker resetActionInvoker;

    @Transactional(value = Transactional.TxType.REQUIRED, rollbackOn = ActionException.class)
    public void reset(ComposedNode composedNode, ResetType resetType) throws ActionException {
        validateResetType(composedNode, resetType);
        Blade blade = getComposedNodeBlade(composedNode);
        resetActionInvoker.reset(blade, resetType);
        updateNodeStateAfterReset(composedNode, resetType);
    }

    private Blade getComposedNodeBlade(ComposedNode composedNode) throws ActionException {
        Blade blade = composedNode.getBlade();
        if (blade == null) {
            throw new ActionException("No Blade is associated with Composed Node. Action aborted");
        }
        return blade;
    }

    // TODO implementation for rest of reset types
    private void validateResetType(ComposedNode composedNode, ResetType resetType) throws ActionException {
        if (!SUPPORTED_RESET_TYPES.contains(resetType)) {
            throw new ActionException("Unsupported reset type: " + resetType);
        }

        ComposedNodeState currentNodeState = composedNode.getComposedNodeState();
        if (currentNodeState != ComposedNodeState.POWERED_OFF && currentNodeState != ComposedNodeState.POWERED_ON) {
            throw new ActionException("Node state invalid for power action: " + composedNode.getComposedNodeState());
        }
    }

    private void updateNodeStateAfterReset(ComposedNode composedNode, ResetType resetType) throws ActionException {
        switch (resetType) {
            case ON:
                composedNode.setComposedNodeState(POWERED_ON);
                break;
            case FORCE_OFF:
            case SOFT_SHUTDOWN:
                composedNode.setComposedNodeState(POWERED_OFF);
                break;
            default:
                throw new ActionException("Unknown reset type: " + resetType);
        }
    }
}
