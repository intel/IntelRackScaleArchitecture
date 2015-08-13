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
 * @file status.hpp
 *
 * @brief Declaration of Status and StatusWithRollup classes.
 * */

#ifndef STATE_TYPE_HPP
#define STATE_TYPE_HPP

#include "psme/rest/utils/enum_chars.hpp"
#include "json/json.hpp"

/*! Psme namespace */
namespace psme {
/*! Rest namespace */
namespace rest {
/*! Resource namespace */
namespace resource {

/*!
 * @enum StateType
 * @brief Type of resource's state
 *
 * @var StateType::ENABLED
 * Enabled state type.
 *
 * @var StateType::DISABLED
 * Disabled state type.
 *
 * @var StateType::OFFLINE
 * Offline state type.
 *
 * @var StateType::IN_TEST
 * InTest state type.
 *
 * @var StateType::STARTING
 * Starting state type.
 *
 * @var StateType::ABSENT
 * Absent state type.
 * */
enum class StateType {
    ENABLED,
    DISABLED,
    OFFLINE,
    IN_TEST,
    STARTING,
    ABSENT
};

/*!
 * @enum HealthType
 * @brief Resource's health type
 *
 * @var HealthType::OK
 * OK health type.
 *
 * @var HealthType::WARNING
 * Warning health type.
 *
 * @var HealthType::CRITICAL
 * Critical health type.
 * */
enum class HealthType {
    OK,
    WARNING,
    CRITICAL
};

/*!
 * @enum EnumStatus
 * @brief Resource's enumeration status
 *
 * @var EnumStatus ENUMERATED
 * Resource is enumerated.
 *
 * @var EnumStatus NOT_ENUMERATED
 * Resource is not enumerated.
 * */
enum class EnumStatus {
    ENUMERATED,
    NOT_ENUMERATED
};

/*!
 * @brief Class holding JSON representation of Status.
 *
 * */
class Status {
public:
    /*! @brief Resource's status property */
    static constexpr char STATUS[] = "Status";
    /*! @brief Known state of the resource */
    static constexpr char STATE[] = "State";
    /*! @brief Overall health state from the view of this resource */
    static constexpr char HEALTH[] = "Health";

    /*! @brief Constructor */
    Status();

    /*! @brief Constructor
     *
     * @param[in] state StateType value.
     * @param[in] health HealthType value.
     */
    Status(StateType state, HealthType health);

    /*! @brief Destructor */
    virtual ~Status();

    /*! @brief Copy Constructor */
    Status(const Status&) = default;

    /*! @brief Assignment operator */
    Status& operator=(const Status&) = default;

    /*!
     * @brief Status state getter.
     *
     * @return Status state.
     */
    StateType get_state() const {
        return m_state;
    }

    /*!
     * @brief Status state setter.
     *
     * @param[in] state StateType value.
     */
    void set_state(StateType state) {
        m_state = state;
    }

    /*!
     * @brief Status health getter.
     *
     * @return Status health.
     */
    HealthType get_health() const {
        return m_health;
    }

    /*!
     * @brief Status health setter.
     *
     * @param[in] health HealthType value.
     */
    void set_health(HealthType health) {
        m_health = health;
    }

    /*!
     * @brief Status JSON object getter.
     *
     * @return Status as JSON object.
     * */
    virtual json::Value as_json() const;
private:
    StateType m_state;
    HealthType m_health;

};

/*!
 * @brief Class holding JSON representation of Status with HealthRollup property.
 *
 * */
class StatusWithRollup : public Status {
public:
    /*!
     * @brief Health state of resource in the absence of its dependent resources
     * */
    static constexpr char HEALTH_ROLLUP[] = "HealthRollup";

    /*! @brief Constructor */
    StatusWithRollup();

    /*! @brief Constructor
     *
     * @param[in] state StateType value.
     * @param[in] health HealthType value.
     * @param[in] health_rollup HealthType value.
     */
    StatusWithRollup(StateType state, HealthType health, HealthType health_rollup);

    /*!
     * @brief Status health rolluo getter.
     *
     * @return Status health rollup.
     */
    HealthType get_health_rollup() const {
        return m_health_rollup;
    }

    /*!
     * @brief Status health rollup setter.
     *
     * @param[in] health HealthType value.
     */
    void set_health_rollup(HealthType health) {
        m_health_rollup = health;
    }

    /*!
     * @brief Status JSON object getter.
     *
     * @return Status as JSON object.
     * */
    json::Value as_json() const override;
private:
    HealthType m_health_rollup;
};

} /*! Resource namespace */
} /*! Rest namespace */
} /*! Psme namespace */

#endif	/* STATE_TYPE_HPP */
