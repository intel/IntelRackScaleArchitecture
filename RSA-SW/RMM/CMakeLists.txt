CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(RMM)
SET(ROOT_DIR ${PROJECT_SOURCE_DIR})

IF(ARM)
	SET (CMAKE_C_COMPILER arm-linux-gcc)
	SET (CMAKE_CXX_COMPILER arm-linux-g++)
	SET (HOST	--host=arm-linux)
	SET (OPENSSL_HOST "no-asm  os/compiler:arm-linux-gcc")
	SET (ZLIB_CC	"CC=arm-linux-gcc")
ENDIF()

exec_program("cd ${PROJECT_SOURCE_DIR}/utils/redfish/mrgenfile/; make clean; make; ./mrgenfile; cd -")

INCLUDE(ExternalProject)
MESSAGE(STATUS "SRC DIR: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "BIN DIR: ${PROJECT_BINARY_DIR}")

IF(EXISTS "${ROOT_DIR}/.svn/")
	FIND_PACKAGE(Subversion)
	IF(SUBVERSION_FOUND)
		Subversion_WC_INFO(${ROOT_DIR} Project)
		SET(SVN_VERSION ${Project_WC_REVISION})
	ENDIF(SUBVERSION_FOUND)
ENDIF(EXISTS "${ROOT_DIR}/.svn/")

if( EXISTS "${ROOT_DIR}/.git" )

execute_process(COMMAND git rev-list --count HEAD
               COMMAND sed -e "s/-/./g"
               OUTPUT_VARIABLE POD_VERSION
               OUTPUT_STRIP_TRAILING_WHITESPACE)
# remove leading "v"
string(REGEX REPLACE "^v(.*)" "\\1" POD_VERSION ${POD_VERSION})
#execute_process( COMMAND bash -c "echo \"${POD_VERSION}\" > ${ROOT_DIR}/version" )
SET(SVN_VERSION ${POD_VERSION})
else( EXISTS "${ROOT_DIR}/.git" )

execute_process(COMMAND cat ${ROOT_DIR}/version
               OUTPUT_VARIABLE POD_VERSION
               OUTPUT_STRIP_TRAILING_WHITESPACE)

endif( EXISTS "${ROOT_DIR}/.git" )


IF(ARM)
	SET(CMAKE_FIND_ROOT_PATH /opt/cc)
	# search for programs in the build host directories
	SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	# for libraries and headers in the target directories
	SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
ENDIF()


file(STRINGS src/VERSION FIRMWARE_VERSION)
SET(CMAKE_C_FLAGS "-g -Wno-unused-variable -Wl,-rpath=/usr/local/lib -Wall -Werror -DRSA_RMM_FIRMWARE_VERSION='\"${FIRMWARE_VERSION}\"' -fstack-protector -fPIC -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security")
SET(CMAKE_LD_FLAGS "-z noexecstack -z relro -z now -pie -rpath=/usr/local/lib")
ADD_SUBDIRECTORY(src)
#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/common)
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

