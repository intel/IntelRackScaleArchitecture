#!/usr/bin/env bash

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

set -e -o pipefail -u

CURRENT_DIR=$(pwd)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

GCC_C_COMPILER=$(which gcc)
GCC_CXX_COMPILER=$(which g++)
CLANG_C_COMPILER=$(which clang)
CLANG_CXX_COMPILER=$(which clang++)

CMAKE=$(which cmake)
MAKE=$(which make)

function build {
    if [[ -z "$CC" || -z "$CXX" ]]; then
        echo "CC or CXX are not set!"
        return
    fi

    if [[ ! -x "$CC" || ! -x "$CXX" ]]; then
        echo "CC=${CC} or CXX=${CXX} doesn't exist!"
        return
    fi

    if [[ ! -x "$CMAKE" ]]; then
        echo "cmake command doesn't exist!"
        return
    fi

    if [[ ! -x "$MAKE" ]]; then
        echo "make command doesn't exist!"
        return
    fi

    build_dir=$SCRIPT_DIR/build.${1}.$(basename $CC)
    if [ ! -d "$build_dir" ]; then
        mkdir $build_dir
    fi
    cd $build_dir
    $CMAKE -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX \
    -DCMAKE_BUILD_TYPE="${1^}" -DCMAKE_CXX_FLAGS=-m64 -DCMAKE_C_FLAGS=-m64 ..
    $MAKE
    cd $CURRENT_DIR
}

CC=$CLANG_C_COMPILER
CXX=$CLANG_CXX_COMPILER
build debug
build release

CC=$GCC_C_COMPILER
CXX=$GCC_CXX_COMPILER
build debug
build release
build coverage
