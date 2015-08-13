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

import com.intel.rsa.client.api.actions.RsaApiActionException;
import com.intel.rsa.client.api.actions.psme.BootSourceOverrideRequest;
import com.intel.rsa.client.api.actions.psme.BootSourceOverrideRequestFactory;
import com.intel.rsa.client.api.actions.psme.PsmeBladeActions;
import com.intel.rsa.client.api.actions.psme.PsmeBladeActionsFactory;
import com.intel.rsa.common.types.BootSourceState;
import com.intel.rsa.common.types.BootSourceType;
import com.intel.rsa.podm.business.entities.assets.Blade;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import java.net.URI;

import static com.intel.rsa.common.types.BootSourceState.CONTINUOUS;

@RequestScoped
public class BootSourceOverrideInvoker {
    @Inject
    private PsmeBladeActionsFactory actionsFactory;

    @Inject
    private BootSourceOverrideRequestFactory bootSourceOverrideRequestFactory;

    public void overrideBootSource(Blade blade, BootSourceType bootSourceType, BootSourceState bootSourceState)
            throws ActionException {
        BootSourceOverrideRequest request = bootSourceOverrideRequestFactory.create(bootSourceType, bootSourceState);

        try {
            PsmeBladeActions actions = createPsmeBladeActions(blade);

            actions.bootSourceOverride(blade.getSourceUri(), request);
        } catch (RsaApiActionException e) {
            throw new ActionException("Set boot order action on PSME Blade failed");
        }
    }

    public void overrideBootSourcePermanently(Blade blade, BootSourceType bootSourceType) throws ActionException {
        overrideBootSource(blade, bootSourceType, CONTINUOUS);
    }

    private PsmeBladeActions createPsmeBladeActions(Blade blade) {
        URI baseUri = blade.getService().getBaseUri();
        return actionsFactory.create(baseUri);
    }
}
