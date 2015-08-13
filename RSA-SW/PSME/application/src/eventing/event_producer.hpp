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

#ifndef PSME_EVENT_PRODUCER_HPP
#define PSME_EVENT_PRODUCER_HPP

#include <atomic>
#include <unordered_map>
#include <set>
#include <thread>

namespace json {
    /*! Forward declaration */
    class Value;
}

/*! PSME namespace */
namespace psme {
/*! Application namespace */
namespace app {
/*! Eventing namespace */
namespace eventing {

using UuidSet = std::set<std::string>;

/*!
  * @brief EventProducer.
  *
  * Creates events based on data polled from agents.
  * */
class EventProducer {
public:
    /*! @brief Constructor.
     *
     * @param[in] config Configuration (JSON object)
     * */
    EventProducer(const ::json::Value& config);

    /*!
     * @brief Starts thread polling agents
     */
    void start();

    /*!
     * @brief Stops thread polling agents
     */
    void stop();

private:
    void produce();
    void do_poll();
    void do_clean() noexcept;
    const UuidSet& get_stored_uuids(const std::string& gami_id);

private:
    const ::json::Value& m_config;
    std::atomic<bool> m_running;
    std::thread m_thread;
    std::unordered_map<std::string, UuidSet> m_stored_uuids;
};

}
}
}

#endif /* PSME_EVENT_PRODUCER_HPP */
