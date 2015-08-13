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

if (NOT PCA95XX_FOUND)
    find_file(pca95xx_library libpca95xx.a
        PATHS ${CMAKE_BINARY_DIR}/lib
    )
    find_library(pca95xx_library pca95xx)
    find_path(pca95xx_include "pca95xx/pca95xx.h"
        PATHS ${CMAKE_BINARY_DIR}/include
    )

    if (pca95xx_library AND pca95xx_include)
        add_library(pca95xx IMPORTED STATIC)
        set_target_properties(pca95xx PROPERTIES
            IMPORTED_LOCATION ${pca95xx_library}
        )

        get_filename_component(pca95xx_library_dir ${pca95xx_library} DIRECTORY)

        set(PCA95XX_LIBRARIES pca95xx)
        set(PCA95XX_LIBRARY_DIRS ${pca95xx_library_dir})
        set(PCA95XX_INCLUDE_DIRS ${pca95xx_include})
        set(PCA95XX_FOUND TRUE)
    endif()
endif()
