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

package com.intel.rsa.podm.rest.odataid;

import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.ContextType;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import static com.intel.rsa.common.types.Id.id;
import static com.intel.rsa.podm.business.services.context.Context.contextOf;
import static com.intel.rsa.podm.business.services.context.ContextType.BLADE;
import static com.intel.rsa.podm.business.services.context.ContextType.COMPUTE_MODULE;
import static com.intel.rsa.podm.business.services.context.ContextType.DRAWER;
import static com.intel.rsa.podm.business.services.context.ContextType.FABRIC_MODULE;
import static com.intel.rsa.podm.business.services.context.ContextType.POD;
import static com.intel.rsa.podm.business.services.context.ContextType.PROCESSOR;
import static com.intel.rsa.podm.business.services.context.ContextType.RACK;
import static com.intel.rsa.podm.business.services.context.ContextType.SWITCH;
import static com.intel.rsa.podm.rest.odataid.ODataIds.oDataIdOfCollectionInContext;
import static org.testng.AssertJUnit.assertEquals;

public class ODataIdsTest {

    @Test
    public void testODataIdOfServiceRoot() throws Exception {
        String result = ODataIds.oDataIdOfServiceRoot().toString();

        assertEquals(result, "/rest/v1");
    }

    @Test
    public void testODataIdOfPods() throws Exception {
        String result = oDataIdOfCollectionInContext(null, POD).toString();

        assertEquals(result, "/rest/v1/Pods");
    }

    @Test(dataProvider = "contextAndODataId")
    public void test(Context context, String oDataId) {
        ODataId actualODataId = ODataIds.oDataIdFromContext(context);
        ODataId expectedODataId = oDataId == null
                ? null
                : ODataId.oDataId(oDataId);

        assertEquals(actualODataId, expectedODataId);
    }

    @DataProvider
    public Object[][] contextAndODataId() {
        return new Object[][] {
                {
                        null,
                        null
                },
                {
                        contextOf(id(1), POD),
                        "/rest/v1/Pods/1"
                },
                {
                        contextOf(id(10), POD).child(id(20), RACK).child(id(30), DRAWER).child(id(40), COMPUTE_MODULE).child(id(50), BLADE),
                        "/rest/v1/Pods/10/Racks/20/Drawers/30/ComputeModules/40/Blades/50"
                },
                {
                        contextOf(id(11), POD).child(id(22), RACK).child(id(33), DRAWER).child(id(44), FABRIC_MODULE).child(id(55), SWITCH),
                        "/rest/v1/Pods/11/Racks/22/Drawers/33/FabricModules/44/Switches/55"
                },
                {
                        contextOf(id(100), POD).child(id(1000), RACK),
                        "/rest/v1/Pods/100/Racks/1000"
                },
        };
    }

    @Test(dataProvider = "contextAndCollectionTypeAndODataId")
    public void test(Context context, ContextType collectionType, String oDataId) {
        ODataId actualODataId = oDataIdOfCollectionInContext(context, collectionType);
        ODataId expectedODataId = ODataId.oDataId(oDataId);

        assertEquals(actualODataId, expectedODataId);
    }

    @DataProvider
    public Object[][] contextAndCollectionTypeAndODataId() {
        return new Object[][] {
                {
                        null,
                        POD,
                        "/rest/v1/Pods"
                },
                {
                        contextOf(id(1), POD),
                        RACK,
                        "/rest/v1/Pods/1/Racks"
                },
                {
                        contextOf(id(11), POD).child(id(22), RACK).child(id(33), DRAWER).child(id(44), COMPUTE_MODULE).child(id(55), BLADE),
                        PROCESSOR,
                        "/rest/v1/Pods/11/Racks/22/Drawers/33/ComputeModules/44/Blades/55/Processors"
                },
                {
                        contextOf(id(100), POD).child(id(1000), RACK),
                        DRAWER,
                        "/rest/v1/Pods/100/Racks/1000/Drawers"
                },
        };
    }

}
