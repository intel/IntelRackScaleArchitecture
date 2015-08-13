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

import com.intel.rsa.podm.business.dto.CollectionDtoType;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public final class CollectionTypeToCollectionODataMapping {

    static final Map<CollectionDtoType, ODataForCollection> COLLECTION_TYPE_TO_ODATA_MAPPING;

    static {
        Map<CollectionDtoType, ODataForCollection> collection = new HashMap<>();

        collection.put(CollectionDtoType.Pod, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSAPods")
                .name("RSA POD Collection")
                .oDataType("#RSAPod.1.0.0.RSAPodCollection")
                .build());

        collection.put(CollectionDtoType.Rack, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSAPods/Links/Members/1/Racks")
                .name("RSA Rack Collection")
                .oDataType("#RSARack.1.0.0.RSARackCollection")
                .build());

        collection.put(CollectionDtoType.Drawer, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSADrawers")
                .name("Drawers Collection")
                .oDataType("#RSADrawer.1.0.0.RSADrawerCollection")
                .build());

        collection.put(CollectionDtoType.ComputeModule, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSADrawers/Links/Members/1/ComputeModules")
                .name("Compute Module Collection")
                .oDataType("#RSAComputeModule.1.0.0.RSAComputeModuleCollection")
                .build());

        collection.put(CollectionDtoType.Blade, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSADrawers/Links/Members/1/ComputeModules/1/Blades")
                .name("Blade Collection")
                .oDataType("#RSABlade.1.0.0.RSABladeCollection")
                .build());

        collection.put(CollectionDtoType.Processor, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSAProcessors")
                .name("Processor Collection")
                .oDataType("#RSAProcessor.1.0.0.RSAProcessorCollection")
                .build());

        collection.put(CollectionDtoType.Memory, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSAMemory")
                .name("Memory Collection")
                .oDataType("#RSAMemory.1.0.0.RSAMemoryCollection")
                .build());

        collection.put(CollectionDtoType.Manager, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSAManagers")
                .name("RSA Manager Collection")
                .oDataType("#RSAManager.1.0.0.RSAManagerCollection")
                .build());

        collection.put(CollectionDtoType.FabricModule, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSAFabricModules")
                .name("Fabric Modules Collection")
                .oDataType("#RSAFabricModule.1.0.0.RSAFabricModuleCollection")
                .build());

        collection.put(CollectionDtoType.NetworkInterface, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#EthernetInterfaces")
                .name("Ethernet Network Interface Collection")
                .oDataType("#EthernetNetworkInterface.1.0.0.EthernetNetworkInterfaceCollection")
                .build());

        collection.put(CollectionDtoType.Switch, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSASwitches")
                .name("RSA Switches Collection")
                .oDataType("#RSASwitch.1.0.0.RSASwitchesCollection")
                .build());

        collection.put(CollectionDtoType.SwitchPort, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSASwitchPorts")
                .name("RSA Switch Ports Collection")
                .oDataType("#RSASwitchPort.1.0.0.RSASwitchPortsCollection")
                .build());

        collection.put(CollectionDtoType.StorageController, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSAStorageControllers")
                .name("StorageControllers Collection")
                .oDataType("#RSAStorageController.1.0.0.RSAStorageControllersCollection")
                .build());

        collection.put(CollectionDtoType.Drive, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#Drives")
                .name("Drives Collection")
                .oDataType("#Drive.1.0.0.DrivesCollection")
                .build());

        collection.put(CollectionDtoType.PhysicalDrive, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#Drives")
                .name("Physical Drives Collection")
                .oDataType("#PhysicalDrive.1.0.0.PhysicalDriveCollection")
                .build());

        collection.put(CollectionDtoType.LogicalDrive, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSALogicalDrives")
                .name("Logical Drives Collection")
                .oDataType("#RSALogicalDrive.1.0.0.RSALogicalDriveCollection")
                .build());

        collection.put(CollectionDtoType.VlanNetworkInterface, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#VLANNetworkInterfaces")
                .name("VLAN Network Interface Collection")
                .oDataType("#VLANNetworkInterface.1.0.0.VLANNetworkInterfaces")
                .build());

        collection.put(CollectionDtoType.ComposedNode, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSAComposedNodes")
                .name("Composed Node Collection")
                .oDataType("#RSAComposedNode.1.0.0.RSAComposedNodeCollection")
                .build());

        collection.put(CollectionDtoType.StorageService, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSAStorageServices")
                .name("Storage Services Collection")
                .oDataType("#RSAStorageService.1.0.0.RSAStorageServicesCollection")
                .build());

        collection.put(CollectionDtoType.Target, ODataForCollection
                .newBuilder()
                .oDataContext("/rest/v1/$metadata#RSARemoteTargets")
                .name("Remote Targets Collection")
                .oDataType("#RSARemoteTarget.1.0.0.RSARemoteTargetCollection")
                .build());

        COLLECTION_TYPE_TO_ODATA_MAPPING = Collections.unmodifiableMap(collection);
    }

    private CollectionTypeToCollectionODataMapping() {
    }

    public static ODataForCollection getOdataForCollectionType(CollectionDtoType collectionDtoType) {
        if (!COLLECTION_TYPE_TO_ODATA_MAPPING.containsKey(collectionDtoType)) {
            String msg = String.format("Unknown Collection type: '%s'", collectionDtoType);
            throw new IllegalArgumentException(msg);
        }
        return COLLECTION_TYPE_TO_ODATA_MAPPING.get(collectionDtoType);
    }
}
