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

package com.intel.rsa.podm.business.services.context;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import static com.intel.rsa.common.types.Id.id;
import static com.intel.rsa.podm.business.services.context.Context.contextOf;
import static com.intel.rsa.podm.business.services.context.Context.isAcceptableChildOf;
import static com.intel.rsa.podm.business.services.context.ContextType.BLADE;
import static com.intel.rsa.podm.business.services.context.ContextType.COMPUTE_MODULE;
import static com.intel.rsa.podm.business.services.context.ContextType.DRAWER;
import static com.intel.rsa.podm.business.services.context.ContextType.FABRIC_MODULE;
import static com.intel.rsa.podm.business.services.context.ContextType.POD;
import static com.intel.rsa.podm.business.services.context.ContextType.RACK;
import static org.testng.Assert.assertEquals;
import static org.testng.Assert.assertFalse;
import static org.testng.Assert.assertNotEquals;
import static org.testng.Assert.assertNotNull;
import static org.testng.Assert.assertTrue;

public class ContextTest {
    @Test
    public void whenCreatingPodContext_ShouldNotReturnFalse() {
        assertNotNull(contextOf(id(12), POD));
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenCreatingPodContextWithNullId_ShouldThrow() {
        contextOf(null, POD);
    }

    @DataProvider
    public Object[][] theSameContexts() {
        return new Object[][] {
                {
                        contextOf(id(1), POD),
                        contextOf(id(1), POD)
                },
                {
                        contextOf(id(2), POD).child(id(3), RACK),
                        contextOf(id(2), POD).child(id(3), RACK)
                }
        };
    }

    @Test(dataProvider = "theSameContexts")
    public void WhenComparingTheSameContexts_ShouldReturnTrue(Context left, Context right) {
        assertEquals(left, right);
    }

    @DataProvider
    public Object[][] differentContexts() {
        return new Object[][] {
                {
                        contextOf(id(2), POD).child(id(3), RACK).child(id(4), DRAWER).child(id(20), COMPUTE_MODULE),
                        null
                },
                {
                        contextOf(id(2), POD),
                        contextOf(id(3), POD)
                },
                {
                        contextOf(id(2), POD),
                        contextOf(id(2), POD).child(id(2), RACK)
                },
                {
                        contextOf(id(4), POD).child(id(2), RACK).child(id(3), DRAWER).child(id(5), COMPUTE_MODULE),
                        contextOf(id(4), POD).child(id(2), RACK).child(id(3), DRAWER).child(id(5), FABRIC_MODULE)
                },
                {
                        contextOf(id(4), POD).child(id(1), RACK).child(id(3), DRAWER),
                        contextOf(id(4), POD).child(id(2), RACK).child(id(3), DRAWER)
                },
        };
    }

    @Test(dataProvider = "differentContexts")
    public void WhenComparingDifferentContexts_ShouldReturnFalse(Context left, Context right) {
        assertNotEquals(left, right);
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenCreatingChildContextWithNullType_ShouldThrow() {
        contextOf(id(3), POD).child(id(12), null);
    }

    @Test
    public void whenCreatingChildContext_ShouldNotReturnNull() {
        assertNotNull(contextOf(id(3), POD).child(id(12), RACK));
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenCreatingRackChildFromRack_ShouldThrow() {
        contextOf(id(1), POD).child(id(2), RACK).child(id(2), RACK);
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenCreatingDrawerChildFromComputeModule_ShouldThrow() {
        contextOf(id(1), POD)
                .child(id(2), RACK)
                .child(id(2), DRAWER)
                .child(id(5), COMPUTE_MODULE)
                .child(id(5), DRAWER);
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void whenCheckingAcceptableChildForNullChildType_shouldThrow() {
        isAcceptableChildOf(null, contextOf(id(1), POD));
    }


    @Test(dataProvider = "acceptableChildren")
    public void testAcceptableChild(ContextType childType, Context parent) {
        assertTrue(isAcceptableChildOf(childType, parent));
    }


    @DataProvider
    public Object[][] acceptableChildren() {
        return new Object[][] {
                {
                        RACK,
                        contextOf(id(1), POD)
                },
                {
                        POD,
                        null
                },
                {
                        BLADE,
                        contextOf(id(1), POD).child(id(2), RACK).child(id(3), DRAWER).child(id(4), COMPUTE_MODULE)
                }
        };
    }

    @Test(dataProvider = "unacceptableChildren")
    public void testUnacceptableChild(ContextType childType, Context parent) {
        assertFalse(isAcceptableChildOf(childType, parent));
    }

    @DataProvider
    public Object[][] unacceptableChildren() {
        return new Object[][] {
                {
                        DRAWER,
                        contextOf(id(1), POD)
                },
                {
                        RACK,
                        contextOf(id(1), POD).child(id(2), RACK)
                },
                {
                        RACK,
                        null
                }
        };
    }
}
