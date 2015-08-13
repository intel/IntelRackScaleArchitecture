/*!
 * @copyright
 * Copyright (c) 2015 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * @file enum_chars.hpp
 *
 * @brief Provides functions to convert enum value to string
 * */

#ifndef ENUM_STRINGIZER_HPP
#define ENUM_STRINGIZER_HPP

#include <stddef.h>
#include <type_traits>
#include <cstring>

namespace psme {
namespace rest {
namespace utils {

/*!
 * @brief Converts enum value to underlying integral type
 *
 * @param e Enum value
 *
 * @return Enum value as integral value
 */
template<typename E>
constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
{
    return static_cast<typename std::underlying_type<E>::type>(e);
}

/*!
 * @brief Gets array size
 *
 * @return Size of array
 */
template<typename T, size_t N>
constexpr size_t array_size(T (&)[N]) { return N; }

/*! @brief Structure holds enum values and their string counterpart */
template<typename T>
struct EnumChars {
    /*! @brief Enum values */
    T m_enum;
    /*! @brief String values */
    const char* m_chars;
};

/*!
 * @brief Gets array of EnumChars
 *
 * @param[out] arr Array to be filled
 * @param[out] size Size of array
 * */
template<typename T>
void get_enum_chars_array(EnumChars<T>*& arr, size_t& size);

/*!
 * @brief Converts enum value to string
 *
 * @param e Enum value
 *
 * @return Enum value as string on success or "Undefined" otherwise
 * */
template<typename T>
const char* to_string(T e) {
    EnumChars<T>* arr;
    size_t arr_size;
    get_enum_chars_array<T>(arr, arr_size);

    for (size_t i = 0; i < arr_size; ++i) {
        if (arr[i].m_enum == e) {
            return arr[i].m_chars;
        }
    }
    return "Undefined";
}

/*!
 * @brief Converts string to enum value
 *
 * @param value String value
 * @param default_value Default value to return in case of failure
 *
 * @return Enum value on success or default_value otherwise
 * */
template<typename T>
T from_string(const char* value, T default_value) {
    EnumChars<T>* arr;
    size_t arr_size;
    get_enum_chars_array<T>(arr, arr_size);

    for (size_t i = 0; i < arr_size; ++i) {
        if (0 == strcmp(arr[i].m_chars, value)) {
            return arr[i].m_enum;
        }
    }
    return default_value;
}

}
}
}
#endif	/* ENUM_STRINGIZER_HPP */

