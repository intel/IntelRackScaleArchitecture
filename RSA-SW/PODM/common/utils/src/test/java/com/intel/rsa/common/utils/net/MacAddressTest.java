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

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import static org.testng.AssertJUnit.assertEquals;

public class MacAddressTest {

    private static final byte HEX_255 = (byte) 0xFF;

    @DataProvider(name = "1")
    public static Object[][] macAddresses() {
        return new Object[][] {
               {new byte[] {0, 1, 2, 3, 4, 5 }, "00:01:02:03:04:05" },
               {new byte[] {5, 4, 3, 2, 1, 0 }, "05:04:03:02:01:00" },
               {new byte[] {HEX_255, HEX_255, HEX_255, HEX_255, HEX_255, HEX_255 }, "FF:FF:FF:FF:FF:FF" }
        };
    }

    @Test(dataProvider = "1")
    public void givenValidMacAddressInByteFormat_ShouldReturnCorrespondingStringRepresentation(byte[] macAddress, String expected) {
        MacAddress sut = new MacAddress(macAddress);

        String actual = sut.toString();

        assertEquals(expected, actual);
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void givenTooLongByteRepresentationOfMacAddress_ShouldThrow() {
        byte[] bytes = {0, 1, 2, 3, 4, 5, 6};

        new MacAddress(bytes);
    }

    @Test(expectedExceptions = IllegalArgumentException.class)
    public void givenTooShortByteRepresentationOfMacAddress_ShouldThrow() {
        byte[] bytes = {0, 1, 2, 3, 4};

        new MacAddress(bytes);
    }

    @Test(expectedExceptions = NullPointerException.class, expectedExceptionsMessageRegExp = ".*macAddress.*")
    public void givenNullByteRepresentationOfMacAddress_ShouldThrow() {
        new MacAddress(null);
    }
}
