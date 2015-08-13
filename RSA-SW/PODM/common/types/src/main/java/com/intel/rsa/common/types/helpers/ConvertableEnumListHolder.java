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

package com.intel.rsa.common.types.helpers;

import com.google.common.base.Splitter;
import com.google.common.base.Strings;
import com.intel.rsa.common.types.ConvertableEnum;

import java.util.LinkedList;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

import static java.util.Collections.unmodifiableList;

public class ConvertableEnumListHolder<T extends ConvertableEnum> {
    public static final String ELEMENT_SEPARATOR = ",";
    private final Class<T> enumType;
    private final List<T> elements;

    public ConvertableEnumListHolder(Class<T> enumType, List<T> elements) {
        this.enumType = enumType;
        if (elements == null) {
            this.elements = unmodifiableList(new LinkedList<>());
        } else {
            this.elements = unmodifiableList(new LinkedList<>(elements));
        }
    }

    public ConvertableEnumListHolder(Class<T> enumType, String text) {
        this.enumType = enumType;
        if (Strings.isNullOrEmpty(text)) {
            this.elements = unmodifiableList(new LinkedList<>());
        } else {
            List<String> elementsStringList = Splitter
                    .on(ELEMENT_SEPARATOR)
                    .trimResults()
                    .splitToList(text);

            List<T> elements = new LinkedList<>();
            for (String element : elementsStringList) {
                elements.add(ConvertableEnumHelper.fromString(this.enumType, element));
            }

            this.elements = unmodifiableList(elements);
        }
    }

    public List<T> toList() {
        return elements;
    }

    public List<String> toStringList() {
        return elements
                .stream()
                .map(T::getValue)
                .collect(Collectors.toCollection(LinkedList::new));
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (!(o instanceof ConvertableEnumListHolder)) {
            return false;
        }

        ConvertableEnumListHolder otherConvertableEnumListHolder = (ConvertableEnumListHolder) o;

        return Objects.equals(elements, otherConvertableEnumListHolder.elements);
    }

    @Override
    public int hashCode() {
        return elements != null ? elements.hashCode() : 0;
    }

    @Override
    public String toString() {
        if (elements == null || elements.isEmpty()) {
            return "";
        }

        return String.join(ELEMENT_SEPARATOR, toStringList());
    }
}
