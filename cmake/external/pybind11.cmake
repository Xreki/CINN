# Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(ExternalProject)

set(PYBIND_SOURCE_DIR ${THIRD_PARTY_PATH}/pybind)

find_package(Python 3.8 COMPONENTS Development)
include_directories(${Python_INCLUDE_DIRS})

message(STATUS "pybind path: ${PYBIND_SOURCE_DIR}/src/extern_pybind/include")
include_directories(${PYBIND_SOURCE_DIR}/src/extern_pybind/include)

# TODO build pybind11 at config time
ExternalProject_Add(
        extern_pybind
        ${EXTERNAL_PROJECT_LOG_ARGS}
        GIT_REPOSITORY  "https://github.com/pybind/pybind11.git"
        GIT_TAG         "v2.4.3"
        PREFIX          ${PYBIND_SOURCE_DIR}
        UPDATE_COMMAND  ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND     ""
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
)

if(${CMAKE_VERSION} VERSION_LESS "3.3.0")
    file(WRITE ${dummyfile} "const char * dummy_pybind = \"${dummyfile}\";")
    add_library(pybind STATIC ${dummyfile})
else()
    add_library(pybind INTERFACE)
endif()
add_dependencies(pybind extern_pybind)
