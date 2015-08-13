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
 * */

#include "command/command.hpp"
#include "command/command_json.hpp"
#include "command/command_json_server.hpp"
#include "logger_ext.hpp"
#include "psme/rest/node/tree_manager.hpp"
#include "psme/rest/http/psme_server_factory.hpp"
#include "psme/rest/http/server.hpp"
#include "registration_server.hpp"
#include "signal.hpp"
#include "eventing_server.hpp"
#include "eventing_data_queue.hpp"
#include "event_producer.hpp"
#include "core/agent/agent_manager.hpp"

#include "configuration/configuration.hpp"
#include "configuration/configuration_validator.hpp"

#include "default_configuration.hpp"
#include "eventing/event_producer.hpp"

#include <jsonrpccpp/server/connectors/httpserver.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

using namespace psme;
using namespace psme::app;
using namespace psme::app::registration;
using namespace psme::rest::http;

using namespace std;

using namespace configuration;

using command::Command;
using command::CommandJson;
using command::CommandJsonServer;

using psme::rest::node::TreeManager;

using namespace logger_cpp;

static constexpr char DEFAULT_COMMANDS_IMPLEMENTATION[] = "Registration";
static constexpr char DEFAULT_SERVER_URL[] = "localhost:8888";
static constexpr unsigned int DEFAULT_REGISTRATION_PORT = 8383;

/*!
 * @brief PSME REST application main method
 * */
int main(int argc, char* argv[]) {
    unsigned int registration_port = DEFAULT_REGISTRATION_PORT;
    std::string server_url = DEFAULT_SERVER_URL;
    std::string commands_implementation = DEFAULT_COMMANDS_IMPLEMENTATION;

    /* Initialize configuration */
    auto& basic_config = configuration::Configuration::get_instance();
    basic_config.set_default_configuration(DEFAULT_CONFIGURATION);
    basic_config.set_default_file(DEFAULT_FILE);
    basic_config.set_default_env_file(DEFAULT_ENV_FILE);
    /* @TODO This should be replaced with nice arguments parsing class */
    while (argc > 1) {
        basic_config.add_file(argv[argc - 1]);
        --argc;
    }
    const json::Value& configuration = basic_config.to_json();

    json::Value json_schema;
    if (configuration::string_to_json(DEFAULT_VALIDATOR_JSON,
                json_schema)) {
        log_info(GET_LOGGER("app"), "JSON Schema load!");

        configuration::SchemaErrors errors;
        configuration::SchemaValidator validator;
        configuration::SchemaReader reader;
        reader.load_schema(json_schema, validator);
        validator.validate(configuration, errors);

        if (!errors.is_valid()) {
            std::cerr << "Configuration invalid: " << errors.to_string() << std::endl;
            return -1;
        }
    }

    LoggerLoader loader(configuration);
    LoggerFactory::instance().set_loggers(loader.load());
    LoggerFactory::set_main_logger_name("app");
    try {
        registration_port = configuration["registration"]["port"].as_uint();
        server_url = configuration["server"]["url"].as_string();
        commands_implementation =
            configuration["commands"]["generic"].as_string();
    } catch (const json::Value::Exception& e) {
        log_error(LOGUSR, "Cannot read setting " << e.what());
    }

    // Create nodes tree and populate with data from getComponents
    TreeManager tree_manager(configuration);

    // Start HTTP server
    unique_ptr<Server> rest_server{PsmeServerFactory::get_instance()
        .create_server(server_url, tree_manager)};

    rest_server->open();

    log_info(GET_LOGGER("rest"), "HTTP server started at address: " << server_url);

    // Starting registration server
    CommandJson::Map::set_implementation(commands_implementation);

    jsonrpc::HttpServer http_server((int(registration_port)));
    RegistrationServer reg_server(http_server);

    reg_server.add(CommandJson::Map::get_instance());
    reg_server.start();

    log_info(LOGUSR, "Registration server started at port: "<< registration_port);

    psme::app::eventing::EventingServer eventing_server{configuration};
    CommandJson::Map::set_implementation("Eventing");
    eventing_server.add(CommandJson::Map::get_instance());
    eventing_server.start();

    psme::app::eventing::EventProducer event_producer(configuration);
    event_producer.start();

    tree_manager.start();

    // Stop the program and wait for interrupt.
    wait_for_interrupt();
    log_info(LOGUSR, "Stopping PSME Application...");

    tree_manager.stop();
    reg_server.stop();
    eventing_server.stop();
    event_producer.stop();

    command::Command::Map::cleanup();
    command::CommandJson::Map::cleanup();
    Configuration::cleanup();
    log_info(LOGUSR, "Registration server stopped.");

    rest_server->close();
    log_info(LOGUSR, "HTTP server stopped.");
    psme::app::eventing::EventingDataQueue::cleanup();
    psme::core::agent::AgentManager::cleanup();
    LoggerFactory::cleanup();
    return 0;
}
