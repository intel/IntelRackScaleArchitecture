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
 * @file command/compute/get_compute_info.hpp
 * @brief Generic compute GetComputeInfo command
 * */

#ifndef AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COMPUTE_INFO_HPP
#define AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COMPUTE_INFO_HPP

#include "agent-framework/command/command.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/fru_info.hpp"
#include "agent-framework/module/oem_data.hpp"

#include <string>

namespace agent_framework {
namespace command {
namespace compute {

/* Forward declaration */
namespace json { class GetComputeInfo; }

/*! Generic compute command GetComputeInfo */
class GetComputeInfo : public Command {
public:
    class Request;
    class Response;

    /*! Tag string for identify agent */
    static constexpr const char AGENT[] = "Compute";

    /*! Tag string for identify command */
    static constexpr const char TAG[] = "getComputeInfo";

    /*!
     * @brief Execute command with given request and response argument
     *
     * @param[in]   request     Input request argument
     * @param[out]  response    Output response argument
     * */
    virtual void execute(const Request& request, Response& response) = 0;

    /*! Command destructor */
    virtual ~GetComputeInfo();
protected:
    /*!
     * @brief Execute command with givent command arguments
     *
     * @param[in]   in      Input command argument
     * @param[out]  out     Output command argument
     * */
    void execute(const Argument& in, Argument& out) override final {
        execute(static_cast<const Request&>(in), static_cast<Response&>(out));
    }
public:
    /*! Argument request to execute */
    class Request : public Argument {
    private:
        friend class json::GetComputeInfo;
        string m_compute{};
    public:
        /*!
         * @brief Get compute from request
         * @return uuid string
         * */
        const string& get_compute() const {
            return m_compute;
        }

        ~Request();
    };

    /*! Argument response from execute */
    class Response : public Argument {
    public:
        /*!
         * @brief Set status from request
         * @param status compute status
         */
        void set_status(const generic::Status& status) {
            m_status = status;
        }

        /*!
         * @brief Set FRUInfo structure
         * @param fru_info compute FRUInfo
         */
        void set_fru_info(const generic::FruInfo& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * @brief Set power zone
         * @param power_zone Power zone
         */
        void set_power_zone(const std::string power_zone) {
            m_power_zone = power_zone;
        }

        /*!
         * @brief Set thermal zone
         * @param thermal_zone Thermal zone
         */
        void set_thermal_zone(const std::string& thermal_zone) {
            m_thermal_zone = thermal_zone;
        }

        /*!
         * @brief Set compute slot
         * @param slot Compute slot
         */
        void set_slot(const std::uint32_t slot) {
            m_slot = slot;
        }

        /*!
         * @brief Set OEMData structure
         * @param oem compute FRUInfo
         */
        void set_oem(const generic::OEMData& oem) {
            m_oem = oem;
        }

        ~Response();

    private:
        friend class json::GetComputeInfo;
        generic::Status m_status{};
        generic::FruInfo m_fru_info{};
        std::string m_power_zone{};
        std::string m_thermal_zone{};
        std::uint32_t m_slot{};
        generic::OEMData m_oem{};
    };
};

}
}
}

#endif /* AGENT_FRAMEWORK_COMMAND_COMPUTE_GET_COMPUTE_INFO_HPP */
