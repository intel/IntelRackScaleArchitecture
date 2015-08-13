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
 * @file module_status.hpp
 * @brief Abstract ModuleStatus
 * */

#ifndef AGENT_FRAMEWORK_STATUS_MODULE_STATUS_HPP
#define AGENT_FRAMEWORK_STATUS_MODULE_STATUS_HPP

// Standard library.
#include <string>

/*! Psme namespace */
namespace agent_framework {
/*! Generic namespace */
namespace generic {

/*!
 * @brief ModuleStatus interafce.
 * */
class ModuleStatus {
public:
    /*!
     * @enum Status
     * @brief Module`s status.
     *
     * @var Status ModuleStatus::UNKNOWN
     * Agent is can not check status of device.
     *
     * @var Status ModuleStatus::PRESENT
     * Device is present.
     *
     * @var Status ModuleStatus::NOT_PRESENT
     * Device is not present.
     * */
    enum class Status {
        UNKNOWN,
        PRESENT,
        NOT_PRESENT
    };
private:
    ModuleStatus(const ModuleStatus &MS);
    ModuleStatus & operator=(const ModuleStatus &MS);
protected:
    /*!
     * @brief Value of a Status.
     * */
    Status m_status = Status::PRESENT;

    /*!
     * @brief IP address of this module.
     * */
    std::string m_ip_address = {};
public:
    /*!
     * @brief Default class constructor.
     * */
    ModuleStatus() {}

    /*!
     * @brief Class destructor.
     * */
    virtual ~ModuleStatus();

    /*!
     * @brief Virtual interafce for reading current ModuleStatus.
     * @return Current ModuleStatus.
     * */
    virtual Status read_status() = 0;

    /*!
     * @brief Getter method for ModuleStatus.
     * @return Status enum.
     * */
    Status inline get_status() const { return m_status; }

    /*!
     * @brief Getter method for IP address.
     * @return std::string IP address of this module..
     * */
    inline std::string get_ip() const { return m_ip_address; }

    /*!
     * @brief Setter method for IP address.
     *
     * @param ip reference to string with IP address.
     * */
    inline void set_ip(const std::string & ip) { m_ip_address = ip; }

};

}
}
#endif /* AGENT_FRAMEWORK_STATUS_MODULE_STATUS_HPP */
