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

package com.intel.rsa.client.deserializers;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;
import com.intel.rsa.common.types.ConvertableEnum;
import com.intel.rsa.common.types.helpers.ConvertableEnumHelper;

import java.io.IOException;

public class ConvertableEnumDeserializer<T extends ConvertableEnum> extends JsonDeserializer<T> {
    private final Class<T> enumType;

    public ConvertableEnumDeserializer(Class<T> enumType) {
        this.enumType = enumType;
    }

    @Override
    public T deserialize(JsonParser parser, DeserializationContext context) throws IOException, JsonProcessingException {
        String text = parser.getValueAsString();

        //Since is optional value it can be null
        if (text == null) {
            return null;
        }

        return ConvertableEnumHelper.fromString(enumType, text);
    }
}
