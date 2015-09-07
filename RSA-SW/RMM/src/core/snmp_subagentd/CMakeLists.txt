SET(TARGET snmp_subagentd)

AUX_SOURCE_DIRECTORY(. SRC_LIST)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
SET(SNMP_AGENT_NEED_LIBS libinit.so  librmmcfg.so libredfish.so libwrap.so liblog.so libutils.so  libjsonrpcapi.so libjsonrpc.so libjson.so libnetsnmpmibs.a libnetsnmpagent.a libnetsnmp.a libcrypto.so libssl.so libcurl.so libjsonrpcapi.so)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include)

ADD_EXECUTABLE(${TARGET} ${SRC_LIST})
ADD_DEPENDENCIES(${TARGET} memdb libjson)
ADD_DEPENDENCIES(${TARGET} ipmi libredfish librmmcfg liblog libutils)
TARGET_LINK_LIBRARIES(${TARGET} ${SNMP_AGENT_NEED_LIBS} libdl.so)
