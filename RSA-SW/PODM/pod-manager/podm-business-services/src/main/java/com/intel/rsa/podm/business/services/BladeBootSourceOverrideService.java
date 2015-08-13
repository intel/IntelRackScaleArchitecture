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

import com.intel.rsa.common.types.BootSourceState;
import com.intel.rsa.common.types.BootSourceType;
import com.intel.rsa.podm.actions.ActionException;
import com.intel.rsa.podm.actions.BootSourceOverrideInvoker;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class BladeBootSourceOverrideService {

    @Inject
    private DomainObjectTreeTraverser traverser;

    @Inject
    private BootSourceOverrideInvoker invoker;

    public void overrideBootSource(Context context, BootSourceType type, BootSourceState state)
            throws EntityNotFoundException, ActionException {
        Blade blade = (Blade) traverser.traverse(context);

        invoker.overrideBootSource(blade, type, state);
    }
}
