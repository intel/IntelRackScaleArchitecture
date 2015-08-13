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

#include "fru_info_dto.hpp"

using namespace psme::core::dto;

void FRUInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_serial_number(response["serialNumber"].asString());
        set_manufacturer(response["manufacturer"].asString());
        set_model_number(response["modelNumber"].asString());
        set_part_number(response["partNumber"].asString());
    }
}

FRUInfoDTO::~FRUInfoDTO(){}

FRUInfoDTO::Response::~Response(){}
