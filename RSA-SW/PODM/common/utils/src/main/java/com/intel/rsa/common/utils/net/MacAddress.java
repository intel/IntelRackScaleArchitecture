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

package com.intel.rsa.common.utils.net;

import static com.google.common.base.Preconditions.checkNotNull;

/**
 * Provides string representations to provided mac address in byte array format
 * Immutable MacAddress but parameter is not defense copied so any changes to the parameter are
 * reflected in string representation
 */
public class MacAddress {
    private static final int MAC_LENGTH = 6;
    private final byte[] macAddress;

    public MacAddress(byte[] macAddress) {
        checkNotNull(macAddress, "macAddress should not be null");

        if (macAddress.length != MAC_LENGTH) {
            throw new IllegalArgumentException("wrong macAddress length");
        }

        this.macAddress = macAddress.clone();
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(String.format("%02X", macAddress[0]));
        for (int i = 1; i < macAddress.length; ++i) {
            sb.append(String.format("%s%02X", ":", macAddress[i]));
        }
        return sb.toString();
    }
}
