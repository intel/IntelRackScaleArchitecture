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
 * @file state_machine_thread.hpp
 * @brief StateMachine thread.
 * */

#ifndef AGENT_FRAMEWORK_STATE_MACHINE_STATE_MACHINE_THREAD_HPP
#define AGENT_FRAMEWORK_STATE_MACHINE_STATE_MACHINE_THREAD_HPP

#include "agent-framework/logger_ext.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "agent-framework/eventing/event_publisher.hpp"
#include "agent-framework/discovery/discovery_manager.hpp"

#include <chrono>
#include <ctime>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <atomic>

using std::unique_ptr;
using agent_framework::generic::ModuleState;
using module_vec_t = agent_framework::generic::ModuleManager::module_vec_t;

/*! Psme namespace */
namespace agent_framework {
/*! Generic namespace */
namespace generic {

class EventMsg;

/* @TODO: This variable has to be moved to configuration class. */
/*! State Machine`s iterating interval. */
const int STATE_MACHINE_INTERVAL_SECONDS = 10;

/*! StateMachine thread. */
class StateMachineThread : public EventPublisher {
    std::thread m_thread;
    std::condition_variable m_condition;
    std::mutex m_mutex;
    volatile bool m_is_running;
    const module_vec_t & m_modules;
    const DiscoveryManager& m_discovery_manager;

    void m_module_init_all();
    void m_module_clean_all();
    void m_task();

    StateMachineThread(StateMachineThread const & smt);
    StateMachineThread& operator=(StateMachineThread const & rhs);

public:
    /*!
     * Default constructor.
     * @param[in] modules   Vector of Modules to start.
     * @param[in] mgr       Discovery manager object
     * */
    StateMachineThread(const module_vec_t & modules, const DiscoveryManager& mgr) :
                           EventPublisher(),
                           m_thread(),
                           m_condition(),
                           m_mutex(),
                           m_is_running(false),
                           m_modules(modules),
                           m_discovery_manager(mgr){}

    /*! Default destructor. */
    ~StateMachineThread();

    /*!
     * @brief Start state machine thread
     */
    void start();


    void set_discovery_manager() {
        //
    }

private:
    /*!
     * @brief Create event message for module
     *
     * @param module System module
     *
     * @return Event message
     */
    EventMsg create_event_msg(Module* module);

    void perform_discovery_if_present(Module & module);
};

/*! State Machine Thread unique pointer */
typedef std::unique_ptr<StateMachineThread> StateMachineThreadUniquePtr;

}
}
#endif /* AGENT_FRAMEWORK_STATE_MACHINE_STATE_MACHINE_THREAD_HPP */

