/*!
 * @section LICENSE
 *
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
 * @section DESCRIPTION
*/

#ifndef PSME_SERVICE_FACTORY_HPP
#define PSME_SERVICE_FACTORY_HPP

#include <string>
#include <vector>

namespace psme {
namespace core {
/*! Agents services */
namespace service {

class ComputeService;
class StorageService;
class NetworkService;

/*! Factory class for creating specified agent service */
class ServiceFactory {
public:
    /*!
     * @brief Create ComputeService for given id
     *
     * @param gami_id agent id
     *
     * @return ComputeService object
     */
    static ComputeService create_compute(const std::string& gami_id);

    /*!
     * @brief Create StorageService for given id
     *
     * @param gami_id agent id
     *
     * @return StorageService object
     */
    static StorageService create_storage(const std::string& gami_id);

    /*!
     * @brief Create NetworkService for given id
     *
     * @param gami_id agent id
     *
     * @return NetworkService object
     */
    static NetworkService create_network(const std::string& gami_id);


    /*!
     * @brief Return all connected computes to PSME Application
     *
     * @return Computes array
     */
    static std::vector<ComputeService> get_all_computes();
};

}
}
}
#endif /* PSME_SERVICE_FACTORY_HPP */
