SET(TARGET_LIB jsonrpcapi)
SET(TARGET_TEST test_jipmi)
SET(TARGET_TESTD test_jipmid)

SET(SRC_LIB jipmi.c memdb.c jsonrpcapi.c assetd_socket.c assetd_api.c registerd_api.c parser.c asset_module_api.c asset_module_socket.c registerd_socket.c utils.c)
SET(SRC_TEST testjipmi.c)
SET(SRC_TESTD testjipmid.c)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include)

ADD_LIBRARY(${TARGET_LIB} SHARED ${SRC_LIB})
TARGET_LINK_LIBRARIES(${TARGET_LIB} liblog.so libpthread.so libutils.so librt.so libjsonrpc.so)

ADD_EXECUTABLE(${TARGET_TEST} ${SRC_TEST})
ADD_DEPENDENCIES(${TARGET_TEST} json jsonrpc log utils)
TARGET_LINK_LIBRARIES(${TARGET_TEST} libjson.so liblog.so libutils.so libjsonrpc.so libjsonrpcapi.so libssl.so libcrypto.so librmmcfg.so libcurl.so )

ADD_EXECUTABLE(${TARGET_TESTD} ${SRC_TESTD})
ADD_DEPENDENCIES(${TARGET_TESTD} json jsonrpc log utils)
TARGET_LINK_LIBRARIES(${TARGET_TESTD} libjson.so liblog.so libutils.so libjsonrpc.so libjsonrpcapi.so libssl.so libcrypto.so librmmcfg.so libcurl.so )

