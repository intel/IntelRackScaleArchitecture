SET(TARGET_IPMI_MODULE ipmi_module)

SET(TARGET_JRPC_APP_TEST jrpc_app_test)
SET(SRC_JRPC_APP_TEST jrpc_app_test.c)

SET(SRC_APP main.c event.c util.c subscribe.c app_intf.c ipmb_intf.c ipmb_handler.c rmcp_intf.c rmcp_handler.c rmcp_session.c ipmi20_crypto.c serial_intf.c serial_handler.c ipmi_jrpc.c)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
SET(IPMI_NEED_LIBS libssl.so libcrypto.so libutils.so  liblog.so librmmcfg.so  libcurl.so  libjson.so  libjsonrpc.so)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include)

ADD_EXECUTABLE(${TARGET_IPMI_MODULE} ${SRC_APP})
ADD_EXECUTABLE(${TARGET_JRPC_APP_TEST} ${SRC_JRPC_APP_TEST})

ADD_DEPENDENCIES(${TARGET_IPMI_MODULE} openssl librmmcfg libjson libjsonrpc)
ADD_DEPENDENCIES(${TARGET_JRPC_APP_TEST} openssl)
TARGET_LINK_LIBRARIES(${TARGET_IPMI_MODULE}  libpthread.so librt.so libdl.so ${IPMI_NEED_LIBS})
TARGET_LINK_LIBRARIES(${TARGET_JRPC_APP_TEST} ${IPMI_NEED_LIBS})
