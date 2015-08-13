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

package com.intel.rsa.client.rss;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.api.reader.rss.IscsiAddress;

import java.net.URI;
import java.util.List;
import java.util.stream.Collectors;

@JsonIgnoreProperties(ignoreUnknown = true)
public class IscsiAddressImpl implements IscsiAddress {

    @JsonProperty("TargetLUN")
    List<TargetLUN> targetLUN;

    @JsonProperty("TargetIQN")
    String targetIQN;

    @JsonProperty("TargetPortalIP")
    String targetPortalIP;

    @JsonProperty("TargetPortalPort")
    int targetPortalPort;

    @Override
    //todo do proper mapping here
    public List<Integer> getTargetLUN() {
        return targetLUN.stream().map(lun -> lun.value).collect(Collectors.toList());
    }

    @Override
    public String getTargetIQN() {
            return targetIQN;
    }

    @Override
    public String getTargetPortalIP() {
            return targetPortalIP;
    }

    @Override
    public int getTargetPortalPort() {
            return targetPortalPort;
    }

    @JsonIgnoreProperties(ignoreUnknown = true)
    public static final class TargetLUN {
        @JsonProperty("LUN")
        Integer value;

        @JsonProperty("Drive")
        URI drive;
    }
}
