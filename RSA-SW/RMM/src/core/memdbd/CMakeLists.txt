SET(TARGET_MEM memdbd)
SET(TARGET_DMP dumpmemdb)
SET(TARGET_TEST memdbtest)

SET(SRC_MEM main.c event.c node.c handle.c snap.c memdb_log.c memdb_jrpc.c)
SET(SRC_DMP dump.c)
SET(SRC_TEST test.c)

SET(LIBS ${LIBS}-lpthread -lrt)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
SET(MEMDB_NEED_LIBS libinit.so libredfish.so libjsonrpcapi.so libjsonrpc.so libjson.so libutils.so liblog.so librmmcfg.so libcurl.so)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)

ADD_EXECUTABLE(${TARGET_MEM} ${SRC_MEM})
ADD_DEPENDENCIES(${TARGET_MEM} libmemdb libjson libjsonrpc librmmcfg)
TARGET_LINK_LIBRARIES(${TARGET_MEM} ${MEMDB_NEED_LIBS})

ADD_EXECUTABLE(${TARGET_DMP} ${SRC_DMP})
ADD_DEPENDENCIES(${TARGET_DMP} libmemdb liblog libutils)
TARGET_LINK_LIBRARIES(${TARGET_DMP} ${MEMDB_NEED_LIBS})

ADD_EXECUTABLE(${TARGET_TEST} ${SRC_TEST})
ADD_DEPENDENCIES(${TARGET_TEST} libmemdb libjson libjsonrpc liblog libutils)
TARGET_LINK_LIBRARIES(${TARGET_TEST} ${MEMDB_NEED_LIBS})
