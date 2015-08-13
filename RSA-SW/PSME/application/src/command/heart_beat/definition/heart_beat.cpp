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
 * @file command/heart_beat/definition/heart_beat.cpp
 * @brief Heart beat command definition
 * */

#include "tag.hpp"
#include "command/heart_beat/heart_beat.hpp"
#include "configuration/configuration.hpp"

#include <chrono>

using namespace psme;
using namespace psme::command;
using namespace std::chrono;

/*! HeartBeat agent implementation */
class HeartBeat : public heart_beat::HeartBeat {
public:
    HeartBeat() : heart_beat::HeartBeat(heart_beat::definition::TAG) {
        auto time_clock = steady_clock::now().time_since_epoch();
        m_time_begin = TimeLive(duration_cast<seconds>(time_clock).count());
    }

    using heart_beat::HeartBeat::execute;

    void execute(const Request&, Response& response) {
        auto time_clock = steady_clock::now().time_since_epoch();
        auto time_now = TimeLive(duration_cast<seconds>(time_clock).count());

        response.set_time_live(time_now - m_time_begin);
        response.set_minimum_delay(m_minimum_delay);
    }

    ~HeartBeat();
private:
    TimeLive m_time_begin{0};
    Seconds m_minimum_delay{3};
};

HeartBeat::~HeartBeat() { }

static Command::Register<HeartBeat> g;
