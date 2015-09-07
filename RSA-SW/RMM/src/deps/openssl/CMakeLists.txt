IF(NOT (EXISTS "${ROOT_DIR}/src/deps/openssl/openssl-1.0.1i.tar.gz"))
    message(STATUS "Dependent library checking ...
        -- Doesn't find openssl-1.0.1i.tar.gz
        -- Please manually download and install it to directory ${ROOT_DIR}/src/deps/openssl/
        -- Reference download site: http://www.openssl.org/source/old/1.0.1/openssl-1.0.1i.tar.gz")
    message(FATAL_ERROR "Missing openssl-1.0.1i.tar.gz")
ENDIF(NOT (EXISTS "${ROOT_DIR}/src/deps/openssl/openssl-1.0.1i.tar.gz"))

ExternalProject_Add(openssl
  INSTALL_DIR ${PROJECT_BINARY_DIR}
  PREFIX ${PROJECT_BINARY_DIR}/ssl_build
  URL ${CMAKE_SOURCE_DIR}/src/deps/openssl/openssl-1.0.1i.tar.gz
  SOURCE_DIR ${PROJECT_BINARY_DIR}/ssl
  CONFIGURE_COMMAND ${PROJECT_BINARY_DIR}/ssl/config shared ${OPENSSL_HOST} --prefix=${PROJECT_BINARY_DIR}/ssl_build --openssldir=${PROJECT_BINARY_DIR}/ssl_build
  BUILD_COMMAND make
  BUILD_IN_SOURCE 1
)

IF(OPENSSL_STATIC_LIB EQUAL 1)
        SET (LIB_CRYPTO libcrypto.a)
        SET (LIB_SSL libssl.a)
ELSE(ARM)
        SET (LIB_CRYPTO libcrypto.a)
        SET (LIB_SSL libssl.a)
ELSE()
        SET (LIB_CRYPTO libcrypto.so)
        SET (LIB_SSL libssl.so)
ENDIF()

ExternalProject_Add_Step(openssl install_ssl
  COMMAND rm -rf ${PROJECT_BINARY_DIR}/include/openssl
  COMMAND rm -f ${PROJECT_BINARY_DIR}/lib/${LIB_CRYPTO}
  COMMAND rm -f ${PROJECT_BINARY_DIR}/lib/${LIB_SSL}
  COMMAND mkdir -p ${PROJECT_BINARY_DIR}/include
  COMMAND mkdir -p ${PROJECT_BINARY_DIR}/lib
  COMMAND cp -rf ${PROJECT_BINARY_DIR}/ssl_build/include/openssl ${PROJECT_BINARY_DIR}/include/
  COMMAND cp -f ${PROJECT_BINARY_DIR}/ssl_build/lib/${LIB_CRYPTO}  ${PROJECT_BINARY_DIR}/lib/
  COMMAND cp -f ${PROJECT_BINARY_DIR}/ssl_build/lib/${LIB_SSL}  ${PROJECT_BINARY_DIR}/lib/
  DEPENDEES install
)

