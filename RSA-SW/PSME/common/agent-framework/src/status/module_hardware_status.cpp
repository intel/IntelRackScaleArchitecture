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
 *
 * @file module_hardware_status.cpp
 *
 * @brief Module hardware status implementation
 * */

#include "agent-framework/status/module_hardware_status.hpp"

#include "agent-framework/logger_ext.hpp"

#include "pca95xx/pca95xx.hpp"
#include "configuration/configuration.hpp"

#include <stdexcept>

using configuration::Configuration;

using namespace agent_framework::generic;
using namespace pca95xx_cpp;

template<typename T>
static inline bool is_bit_set(T& value, T bit) {
    return !!(value & (T(1) << bit));
}

template<typename T>
static inline T set_bit(T& value, T bit) {
    value |= (T(1) << bit);
    return value;
}

template<typename T>
static inline T clear_bit(T& value, T bit) {
    value &= ~(T(1) << bit);
    return value;
}

struct ModuleHardwareStatus::GpioSetting {
    unsigned int bus;
    unsigned int pin;
    unsigned int bank;
    unsigned int address;
    bool inverted;
};

void ModuleHardwareStatus::gpio_pca95xx(const char* model_name,
        const GpioSetting& setting) {
    unsigned int value;
    enum pca95xx_model model = Pca95xx::get_model(model_name);
    if (PCA95XX_UNKNOWN == model) {
        m_status = ModuleStatus::Status::UNKNOWN;
        log_warning(GET_LOGGER("status"), model_name << " not supported");
        return;
    }

    try {
        Pca95xx gpio(model);
        gpio.set_i2c_bus_number(setting.bus);
        gpio.set_i2c_slave_address(setting.address);
#if 0
@TODO: Removed unnecessary configuration write as PCA is used in a default cofiguration
        value = gpio.get_config(setting.bank);
        gpio.set_config(setting.bank, set_bit(value, setting.pin));

        value = gpio.get_polarity_inv(setting.bank);
        gpio.set_polarity_inv(setting.bank, setting.inverted ?
            set_bit(value, setting.pin) : clear_bit(value, setting.pin));
#endif
        value = gpio.get_input(setting.bank);
        if (is_bit_set(value, setting.pin)) {
            m_status = ModuleStatus::Status::PRESENT;
        }
        else {
            m_status = ModuleStatus::Status::NOT_PRESENT;
        }
    }
    catch (const std::runtime_error& e) {
        m_status = ModuleStatus::Status::UNKNOWN;
        log_error(GET_LOGGER("status"), model_name << " " << e.what());
    }
    catch (...)  {
        m_status = ModuleStatus::Status::UNKNOWN;
        log_alert(GET_LOGGER("status"), model_name << " unknown error");
    }
}

void ModuleHardwareStatus::gpio_section(const json::Value& value) {
    GpioSetting setting {};

    log_debug(GET_LOGGER("status"), "Found gpio section for module " << m_ip_address);


	for (const auto& pin : value["pins"].as_array()) {
		try {
			setting.bus = value["bus"].as_uint();
			setting.pin = pin.as_uint();
			log_debug(GET_LOGGER("status"), "Checking for pin: " << setting.pin);
			setting.bank = value["bank"].as_uint();
			setting.address = value["address"].as_uint();
			setting.inverted = value["inverted"].as_bool();
			if (0 == value["model"].as_string().compare(0, 5, "PCA95")) {
				gpio_pca95xx(value["model"].as_string().c_str(), setting);
			}
		}
		catch (const json::Value::Exception& e) {
			m_status = ModuleStatus::Status::UNKNOWN;
			log_error(GET_LOGGER("status"), "Invalid/missing gpio member: " << e.what());
		}
		catch (...) {
			m_status = ModuleStatus::Status::UNKNOWN;
			log_alert(GET_LOGGER("status"), "Unknown error in gpio section");
		}
		if (m_status == ModuleStatus::Status::PRESENT) break;
	}
}

ModuleStatus::Status ModuleHardwareStatus::read_status() {
    const json::Value& configuration = Configuration::get_instance().to_json();
    const json::Value& modules = configuration["modules"];

    if (!modules.is_array()) {
        m_status = ModuleStatus::Status::UNKNOWN;
        log_error(GET_LOGGER("status"), "Invalid hardware configuration for modules");
        return m_status;
    }

    for (const auto& module: modules) {
        if (module["ipv4"].is_string() == false) { continue; }
        if (module["ipv4"].as_string() != m_ip_address) { continue; }

        if (module["gpio"].is_object()) {
            gpio_section(module["gpio"]);
        }

        log_debug(GET_LOGGER("status"), "Hardware status " << size_t(m_status)
                << " for " << m_ip_address);
    }

    return m_status;
}
