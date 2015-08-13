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

package com.intel.rsa.podm.business.services.helpers;

import com.intel.rsa.podm.business.dto.RemoteTargetIscsiAddressDto;
import com.intel.rsa.podm.business.dto.RemoteTargetIscsiInitiatorDto;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.assets.RemoteTargetIscsiAddress;
import com.intel.rsa.podm.business.entities.assets.RemoteTargetIscsiInitiator;

import java.util.Collection;
import java.util.LinkedList;
import java.util.List;

public class RemoteTargetDtoMapHelper {
    public RemoteTargetDtoMapHelper() {
    }

    public Collection<RemoteTargetIscsiInitiatorDto> extractIscsiInitiators(RemoteTarget target) {
        List<RemoteTargetIscsiInitiatorDto> result = new LinkedList<RemoteTargetIscsiInitiatorDto>();

        for (RemoteTargetIscsiInitiator initiator : target.getRemoteTargetIscsiInitiators()) {
            RemoteTargetIscsiInitiatorDto.Builder builder = RemoteTargetIscsiInitiatorDto.newBuilder()
                    .initiatorIQN(initiator.getInitiatorIqn());

            RemoteTargetIscsiInitiatorDto dto = builder.build();
            result.add(dto);
        }

        return result;
    }

    public Collection<RemoteTargetIscsiAddressDto> extractIscsiAddresses(RemoteTarget target) {
        List<RemoteTargetIscsiAddressDto> result = new LinkedList<RemoteTargetIscsiAddressDto>();

        for (RemoteTargetIscsiAddress adr : target.getRemoteTargetIscsiAddresses()) {
            RemoteTargetIscsiAddressDto.Builder builder = RemoteTargetIscsiAddressDto.newBuilder()
                    .targetIQN(adr.getTargetIqn())
                    .targetLUN(adr.getTargetLun())
                    .targetPortalIP(adr.getTargetPortalIp())
                    .targetPortalPort(adr.getTargetPortalPort());

            RemoteTargetIscsiAddressDto dto = builder.build();
            result.add(dto);
        }

        return result;
    }
}
