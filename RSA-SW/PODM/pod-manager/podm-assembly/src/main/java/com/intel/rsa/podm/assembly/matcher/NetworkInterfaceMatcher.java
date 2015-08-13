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

package com.intel.rsa.podm.assembly.matcher;

import com.intel.rsa.podm.business.dto.RequestedNetworkInterface;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

public class NetworkInterfaceMatcher {
    public boolean matches(RequestedNode template, Collection<NetworkInterface> networkInterfaces) {
        if (template.getNetworkInterfaces() != null && !template.getNetworkInterfaces().isEmpty()) {
            if  (template.getNetworkInterfaces().size() > networkInterfaces.size()) {
                return false;
            }

            return areTemplateNetworkInterfacesMatchNetworkInterfaces(template.getNetworkInterfaces(), networkInterfaces);
        }

        return true;
    }

    private boolean areTemplateNetworkInterfacesMatchNetworkInterfaces(List<RequestedNetworkInterface> templateInterfaces,
                                                                       Collection<NetworkInterface> networkInterfaces) {
        List<NetworkInterface> filteredNetworkInterfaces = new ArrayList<>(networkInterfaces);
        for (RequestedNetworkInterface templateInterface : templateInterfaces) {
            List<NetworkInterface> localFilteredNetworkInterfaces = new ArrayList<>(filteredNetworkInterfaces);

            localFilteredNetworkInterfaces = filterNetworkInterfacesBySpeed(localFilteredNetworkInterfaces, templateInterface.getSpeedMbps());

            if (localFilteredNetworkInterfaces.isEmpty()) {
                return false;
            }

            Collections.sort(localFilteredNetworkInterfaces, (NetworkInterface n1, NetworkInterface n2) -> n1.getSpeedMbps() - n2.getSpeedMbps());
            filteredNetworkInterfaces.remove(localFilteredNetworkInterfaces.get(0));
        }

        return true;
    }

    private List<NetworkInterface> filterNetworkInterfacesBySpeed(List<NetworkInterface> networkInterfaces, Integer speedMbps) {
        if (speedMbps == null) {
            return networkInterfaces;
        }

        return networkInterfaces.stream()
                .filter(networkInterface -> networkInterface.getSpeedMbps() >= speedMbps)
                .collect(Collectors.toList());
    }
}
