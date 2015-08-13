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

package com.intel.rsa.podm.rest.validators;

import com.intel.rsa.podm.business.dto.RequestedMasterDrive;
import com.intel.rsa.podm.business.dto.RequestedRemoteDrive;

import java.math.BigDecimal;

import static com.intel.rsa.common.types.RemoteStorageType.ISCSI;
import static java.util.Objects.nonNull;

public class RequestedRemoteDrivesValidator {
    public boolean driveIsValid(RequestedRemoteDrive drive) {
        if (isDefiningExistingTarget(drive)) {
            return validForExistingTarget(drive);
        } else {
            return validForNewTarget(drive);
        }
    }

    private boolean validForNewTarget(RequestedRemoteDrive drive) {
        return capacityIsValid(drive.getCapacityGB())
                && nonNull(drive.getAddress())
                && masterIsValid(drive.getMaster());
    }

    private boolean capacityIsValid(BigDecimal capacity) {
        return nonNull(capacity)
                && capacity.signum() != -1;
    }

    private boolean masterIsValid(RequestedMasterDrive masterDrive) {
        return  nonNull(masterDrive)
                && masterDrive.getType() != null
                && ISCSI.equals(masterDrive.getAddressType())
                && nonNull(masterDrive.getAddress());
    }

    private boolean validForExistingTarget(RequestedRemoteDrive drive) {
        return nonNull(drive.getAddress())
                && ISCSI.equals(drive.getAddressType());
    }

    private boolean isDefiningExistingTarget(RequestedRemoteDrive drive) {
        return drive.getMaster() == null;
    }
}
