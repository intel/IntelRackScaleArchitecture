# <license_header>
#
# Copyright (c) 2015 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# </license_header>

if (NOT GTEST_FOUND)
    return()
endif()

add_gtest(eventing_test_queue
    test_runner.cpp
    eventing_test_queue.cpp
)

add_gtest(eventing_test_notification
    test_runner.cpp
    eventing_test_notification.cpp
)

add_gtest(eventing_test_subscription
    test_runner.cpp
    eventing_test_subscription.cpp
)

target_link_libraries(eventing_test_queue
    ${JSONCXX_LIBRARIES}
    ${AGENT_FRAMEWORK_LIB}
    ${UUID_LIBRARIES}
    ${PCA95XX_LIBRARIES}
)
target_link_libraries(eventing_test_notification
    ${JSONCXX_LIBRARIES}
    ${AGENT_FRAMEWORK_LIB}
    ${UUID_LIBRARIES}
    ${PCA95XX_LIBRARIES}
)
target_link_libraries(eventing_test_subscription
    ${JSONCXX_LIBRARIES}
    ${AGENT_FRAMEWORK_LIB}
    ${UUID_LIBRARIES}
    ${PCA95XX_LIBRARIES}
)

target_include_directories(eventing_test_queue PUBLIC
    ${AGENT_FRAMEWORK_DIR}/src/state_machine
)

target_include_directories(eventing_test_subscription PUBLIC
    ${AGENT_FRAMEWORK_DIR}/src/state_machine
)

target_include_directories(eventing_test_notification PUBLIC
    ${AGENT_FRAMEWORK_DIR}/src/state_machine
)




