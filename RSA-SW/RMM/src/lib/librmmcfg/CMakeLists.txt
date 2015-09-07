SET(TARGET_LIB rmmcfg)
SET(TARGET_TEST test_rmmcfg)

SET(SRC_LIB librmmcfg.c)
SET(SRC_TEST librmmcfg.c test.c)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)

ADD_LIBRARY(${TARGET_LIB} SHARED ${SRC_LIB})
ADD_DEPENDENCIES(${TARGET_LIB} utils libencrypter)
TARGET_LINK_LIBRARIES(${TARGET_LIB} libjson.so libutils.so libencrypter.so)

ADD_EXECUTABLE(${TARGET_TEST} ${SRC_TEST})
ADD_DEPENDENCIES(${TARGET_TEST} utils libencrypter)
TARGET_LINK_LIBRARIES(${TARGET_TEST} libjson.so libcurl.so libcrypto.so libssl.so libencrypter.so libutils.so libpthread.so)
