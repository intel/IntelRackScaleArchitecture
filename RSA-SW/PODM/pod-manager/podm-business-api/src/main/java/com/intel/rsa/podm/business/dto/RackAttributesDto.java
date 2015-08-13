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

package com.intel.rsa.podm.business.dto;

import java.util.Collection;

/**
 * Class representing attributes obtained from RMM
 *
 * TODO add other fields that can be obtained from RMM
 */
public final class RackAttributesDto {
    private final boolean isRmmPresent;
    private final Collection<RackTray> traysPresent;

    private RackAttributesDto(Builder builder) {
        isRmmPresent = builder.isRmmPresent;
        traysPresent = builder.traysPresent;
    }

    public boolean isRmmPresent() {
        return isRmmPresent;
    }

    public Collection<RackTray> getTraysPresent() {
        return traysPresent;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    /**
     * Class representing single rack tray identifiable by TrayRUID
     */
    public static final class RackTray {
        private final long trayRuid;

        public RackTray(long trayRuid) {
            this.trayRuid = trayRuid;
        }

        public long getTrayRuid() {
            return trayRuid;
        }
    }

    public static final class Builder {
        private boolean isRmmPresent;
        private Collection<RackTray> traysPresent;

        private Builder() {
        }

        public Builder isRmmPresent(boolean isRmmPresent) {
            this.isRmmPresent = isRmmPresent;
            return this;
        }

        public Builder addPresentTrays(Collection<RackTray> presentTrays) {
            this.traysPresent = presentTrays;
            return this;
        }

        public RackAttributesDto build() {
            return new RackAttributesDto(this);
        }
    }
}
