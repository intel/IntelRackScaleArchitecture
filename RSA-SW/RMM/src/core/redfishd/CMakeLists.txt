SET(TARGET_RFEVT redfishd)
SET(TARGET_RFTEST test_redfishd)
SET(TARGET_SSATEST test_snmp_subagentd)

SET(SRC_RFEVT main.c rf_memdb.c rf_log.c)
SET(SRC_RFTEST test_redfish.c)
SET(SRC_SSATEST test_snmp.c)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

IF(ARM)
	SET(CMAKE_C_COMPILER arm-linux-gcc)
ENDIF()

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include)

ADD_EXECUTABLE(${TARGET_RFEVT} ${SRC_RFEVT})
ADD_DEPENDENCIES(${TARGET_RFEVT} memdb redfish libcurl libutils librmmcfg)
TARGET_LINK_LIBRARIES(${TARGET_RFEVT} libinit.so libjsonrpcapi.so libutils.so libpthread.so libwrap.so libredfish.so libcurl.so liblog.so libjson.so librmmcfg.so)

ADD_EXECUTABLE(${TARGET_RFTEST} ${SRC_RFTEST})
ADD_DEPENDENCIES(${TARGET_RFTEST} redfish)
TARGET_LINK_LIBRARIES(${TARGET_RFTEST} libredfish.so libjsonrpcapi.so libjsonrpc.so libjson.so liblog.so libutils.so librmmcfg.so libcurl.so)

ADD_EXECUTABLE(${TARGET_SSATEST} ${SRC_SSATEST})
ADD_DEPENDENCIES(${TARGET_SSATEST} redfish)
TARGET_LINK_LIBRARIES(${TARGET_SSATEST} libredfish.so libjsonrpcapi.so libjsonrpc.so libjson.so liblog.so libutils.so librmmcfg.so libcurl.so)
