SET(TARGET restd)

SET(SRC_LIST main.c http.c rest.c websocket.c handler/rack_handler.c handler/mzone_handler.c handler/dzone_handler.c handler/pzone_handler.c handler/tzone_handler.c handler/general_handler.c)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include)

ADD_EXECUTABLE(${TARGET} ${SRC_LIST})
ADD_DEPENDENCIES(${TARGET} memdb ipmi json openssl redfish libutils librmmcfg)
TARGET_LINK_LIBRARIES(${TARGET} libinit.so libjson.so libjsonrpcapi.so libpthread.so libssl.so libcrypto.so libwrap.so libredfish.so libwrap.so liblog.so librmmcfg.so libcurl.so libutils.so)

INSTALL(
  DIRECTORY web
  DESTINATION ${PROJECT_BINARY_DIR}/bin
  )
