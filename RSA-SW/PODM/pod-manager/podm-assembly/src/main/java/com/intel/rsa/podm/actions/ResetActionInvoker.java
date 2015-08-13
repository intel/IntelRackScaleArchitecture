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
import com.intel.rsa.client.api.actions.psme.PsmeBladeActions;
import com.intel.rsa.client.api.actions.psme.PsmeBladeActionsFactory;
import com.intel.rsa.client.api.actions.psme.ResetRequest;
import com.intel.rsa.client.api.actions.psme.ResetRequestFactory;
import com.intel.rsa.common.types.actions.ResetType;
import com.intel.rsa.podm.business.entities.assets.Blade;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import java.net.URI;


/**
 * Delegates reset action performed on Blade.
 */
@RequestScoped
public class ResetActionInvoker {
    @Inject
    private PsmeBladeActionsFactory actionsFactory;

    @Inject
    private ResetRequestFactory resetRequestFactory;

    public void powerOn(Blade blade) throws ActionException {
        reset(blade, ResetType.ON);
    }

    public void reset(Blade blade, ResetType resetType) throws ActionException {
        try {
            PsmeBladeActions actions = createPsmeBladeActions(blade);
            ResetRequest resetRequest = resetRequestFactory.create(resetType);
            actions.reset(blade.getSourceUri(), resetRequest);
        } catch (RsaApiActionException e) {
            throw new ActionException("Reset action on PSME Blade failed");
        }
    }

    private PsmeBladeActions createPsmeBladeActions(Blade blade) {
        URI baseUri = blade.getService().getBaseUri();
        return actionsFactory.create(baseUri);
    }

    public void shutdownGracefully(Blade blade) throws ActionException {
        reset(blade, ResetType.SOFT_SHUTDOWN);
    }

    public void powerOff(Blade blade) throws ActionException {
        reset(blade, ResetType.FORCE_OFF);
    }
}
