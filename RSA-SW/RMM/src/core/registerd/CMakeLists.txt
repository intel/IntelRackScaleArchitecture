SET(TARGET registerd)

SET(SRC_LIST main.c)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)

ADD_EXECUTABLE(${TARGET} ${SRC_LIST})
ADD_DEPENDENCIES(${TARGET} memdb libutils)
ADD_DEPENDENCIES(${TARGET} ipmi)
ADD_DEPENDENCIES(${TARGET} libutils wrap librmmcfg libredfish)
TARGET_LINK_LIBRARIES(${TARGET} libjsonrpcapi.so libutils.so libpthread.so libm.so librmmcfg.so  
libwrap.so liblog.so libjsonrpc.so libjson.so libredfish.so libcurl.so)

