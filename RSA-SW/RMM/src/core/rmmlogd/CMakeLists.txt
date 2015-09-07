SET(TARGET_RMM_LOGD rmmlogd)
SET(TARGET_TEST test_rmmlogd)

SET(SRC_EVT_LOG main.c log_manager.c)
SET(SRC_TEST test.c log_manager.c )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)

ADD_EXECUTABLE(${TARGET_RMM_LOGD} ${SRC_EVT_LOG})
ADD_DEPENDENCIES(${TARGET_RMM_LOGD} memdb libutils librmmcfg)
TARGET_LINK_LIBRARIES(${TARGET_RMM_LOGD} libutils.so libpthread.so libjson.so librmmcfg.so libcurl.so)

ADD_EXECUTABLE(${TARGET_TEST} ${SRC_TEST})
ADD_DEPENDENCIES(${TARGET_TEST} eventd librmmcfg) 
TARGET_LINK_LIBRARIES(${TARGET_TEST} libutils.so libpthread.so libjson.so librmmcfg.so libcurl.so)
