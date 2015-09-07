SET(TARGET coolingctrl)
SET(TARGET_TEST test_coolingctrl)

SET(SRC_COOL main.c cooling_ctrl.c)
SET(SRC_TEST test.c)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)

ADD_EXECUTABLE(${TARGET} ${SRC_COOL})
ADD_DEPENDENCIES(${TARGET} memdb)
ADD_DEPENDENCIES(${TARGET} ipmi libutils librmmcfg)
TARGET_LINK_LIBRARIES(${TARGET} libinit.so libredfish.so libjsonrpcapi.so libjsonrpc.so libjson.so libutils.so liblog.so librmmcfg.so libcurl.so)

ADD_EXECUTABLE(${TARGET_TEST} ${SRC_TEST})
ADD_DEPENDENCIES(${TARGET_TEST} utils)
ADD_DEPENDENCIES(${TARGET_TEST} memdb libutils)
TARGET_LINK_LIBRARIES(${TARGET_TEST}  libredfish.so libjsonrpcapi.so libjsonrpc.so libjson.so libutils.so liblog.so libcurl.so)

