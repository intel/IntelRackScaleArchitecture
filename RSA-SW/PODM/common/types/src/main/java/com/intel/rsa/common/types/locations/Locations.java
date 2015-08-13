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

package com.intel.rsa.common.types.locations;

/**
 * Helper methods allowing to obtain instance od Location in functional style.
 */
public final class Locations {
    private Locations() {
    }

    public static Location podLocation(int pod) {
        return new Location(pod, null, null, null, null);
    }

    public static Location rackLocation(int pod, int rack) {
        return new Location(pod, rack, null, null, null);
    }

    public static Location drawerLocation(int pod, int rack, int drawer) {
        return new Location(pod, rack, drawer, null, null);
    }

    public static Location moduleLocation(int pod, int rack, int drawer, int computeModule) {
        return new Location(pod, rack, drawer, computeModule, null);
    }

    public static Location bladeLocation(int pod, int rack, int drawer, int computeModule, int blade) {
        return new Location(pod, rack, drawer, computeModule, blade);
    }
}
