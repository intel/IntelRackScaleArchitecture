# Copyright (c) 2015 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

USER_NAME=rsa

echo_log() {
    echo "[${DPKG_MAINTSCRIPT_PACKAGE}]: ${1}"
}

create_rsa_user() {
    #check if rsa user exist
    echo_log "Creating '${USER_NAME}' user"

    GROUP_NAME=${USER_NAME}
    USER_HOME="/opt/wildfly"

    getent group ${GROUP_NAME} > /dev/null || groupadd --system ${GROUP_NAME}
    getent passwd ${USER_NAME} > /dev/null || useradd --system --gid ${GROUP_NAME} --home ${USER_HOME} --shell /usr/sbin/nologin ${USER_NAME}
}

echo_log "preinst: beginning ${1} action"

create_rsa_user

echo_log "preinst: done"
