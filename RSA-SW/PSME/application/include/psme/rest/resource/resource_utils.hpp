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
 * @file resource_utils.hpp
 *
 * @brief Declaration of ResourceUtils class
 * */
#ifndef PSME_REST_UTILS_RESOURCEUTILS_HPP
#define	PSME_REST_UTILS_RESOURCEUTILS_HPP

#include <string>

namespace psme {
namespace rest {
namespace resource {

/*! @brief Provides methods to get time */
class ResourceUtils {
public:
    /*!
     * @brief Local time getter
     * @return Local time with time zone
     * */
    static std::string get_time_with_zone();

    /*!
     * @brief Local time getter
     * @return Local time
     * */
    static std::string get_time();

private:
    static std::string get_time(const char* format);

    ResourceUtils() = delete;
    ResourceUtils(const ResourceUtils& orig) = delete;
    ResourceUtils& operator=(const ResourceUtils&) = delete;
    virtual ~ResourceUtils() = delete;
};

}
}
}
#endif	/* PSME_REST_UTILS_RESOURCEUTILS_HPP */


