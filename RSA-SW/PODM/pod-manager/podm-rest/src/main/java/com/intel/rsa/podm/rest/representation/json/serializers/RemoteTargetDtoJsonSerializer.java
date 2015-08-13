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

package com.intel.rsa.podm.rest.representation.json.serializers;

import com.intel.rsa.podm.business.dto.RemoteTargetDto;
import com.intel.rsa.podm.business.dto.RemoteTargetIscsiAddressDto;
import com.intel.rsa.podm.business.dto.RemoteTargetIscsiInitiatorDto;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.odataid.ODataIds;
import com.intel.rsa.podm.rest.representation.json.templates.RemoteTargetJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.IscsiAddressJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.IscsiInitiatorJson;

import java.util.ArrayList;
import java.util.stream.Collectors;

import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdFromContext;
import static java.net.URI.create;

public class RemoteTargetDtoJsonSerializer extends DtoJsonSerializer<RemoteTargetDto> {

    public RemoteTargetDtoJsonSerializer() {
        super(RemoteTargetDto.class);
    }

    @Override
    protected RemoteTargetJson translate(RemoteTargetDto dto) {
        RemoteTargetJson json = new RemoteTargetJson();

        json.oDataContext = create("/rest/v1/$metadata#RSARemoteTargets/Links/Members/$entity");
        json.oDataId = ODataId.oDataId(context.getRequestPath());

        json.id = dto.getId();
        json.name = dto.getName();
        json.modified = dto.getModified();
        json.status.state = dto.getState();
        json.status.health = dto.getHealth();
        json.status.healthRollup = dto.getHealthRollup();
        json.enumStatus = dto.getEnumStatus();
        json.type = dto.getType();

        rewriteCollections(dto, json);

        return json;
    }

    private void rewriteCollections(RemoteTargetDto dto, RemoteTargetJson json) {
        rewriteIscsiAddresses(dto, json);
        rewriteInitiators(dto, json);

        json.links.logicalDrives.addAll(dto.getLogicalDrives().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
        json.links.managedBy.addAll(dto.getManagedBy().stream()
                .map(ODataIds::oDataIdFromContext)
                .collect(Collectors.toList()));
    }

    private void rewriteInitiators(RemoteTargetDto dto, RemoteTargetJson json) {
        for (RemoteTargetIscsiInitiatorDto dtoInit : dto.getInitiators()) {
            IscsiInitiatorJson init = new IscsiInitiatorJson();

            init.initiatorIqn = dtoInit.getInitiatorIQN();

            RemoteTargetJson.InitiatorWrapper initiatorWrapper = new RemoteTargetJson.InitiatorWrapper();
            initiatorWrapper.iSCSI = init;

            json.initiator.add(initiatorWrapper);
        }
    }

    private void rewriteIscsiAddresses(RemoteTargetDto dto, RemoteTargetJson json) {
        for (RemoteTargetIscsiAddressDto dtoAddr : dto.getIscsiAddresses()) {
            IscsiAddressJson addr = new IscsiAddressJson();
            addr.targetIQN = dtoAddr.getTargetIQN();
            addr.targetLUN = new ArrayList<>();

            rewriteTargetLUNs(dto, dtoAddr, addr);

            addr.targetPortalIP = dtoAddr.getTargetPortalIP();
            addr.targetPortalPort = dtoAddr.getTargetPortalPort();

            RemoteTargetJson.AddressWrapper addressWrapper = new RemoteTargetJson.AddressWrapper();
            addressWrapper.iSCSI = addr;

            json.addresses.add(addressWrapper);
        }
    }

    private void rewriteTargetLUNs(RemoteTargetDto dto, RemoteTargetIscsiAddressDto dtoAddr, IscsiAddressJson addr) {
        if (dto.getLogicalDrives().isEmpty()) {
            return;
        } else {
            //todo fixme, this is just a temporary solution, disk should be read from "TargetLUN" object
            Context contextOfFirstDrive = dto.getLogicalDrives().stream().findFirst().get();
            String driveUri = oDataIdFromContext(contextOfFirstDrive).toString();
            for (Integer lun : dtoAddr.getTargetLUN()) {
                IscsiAddressJson.TargetLUN targetLUN = new IscsiAddressJson.TargetLUN(lun, driveUri);
                addr.targetLUN.add(targetLUN);
            }
        }
    }
}
