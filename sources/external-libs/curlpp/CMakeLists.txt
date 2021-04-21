include(ExternalProject)

set(CURLPP_ROOT ${CMAKE_BINARY_DIR}/external-libs/curlpp)

list(APPEND CURLPP_BUILD_ARGS "-DCURL_INCLUDE_DIR=${CURL_INCLUDE_DIR}")
list(APPEND CURLPP_BUILD_ARGS "-DCURL_LIBRARY=libcurl")
list(APPEND CURLPP_BUILD_ARGS "-DCURLPP_BUILD_SHARED_LIBS=OFF")
if(WIN32)
   list(APPEND CURLPP_BUILD_ARGS "-DCURLPP_STATIC_CRT=ON")
else()
   list(APPEND CURLPP_BUILD_ARGS "-DCMAKE_POSITION_INDEPENDENT_CODE=ON")
endif()


if(CMAKE_TOOLCHAIN_FILE)
   message("CURLPP : cross compilation (toolchain:${CMAKE_TOOLCHAIN_FILE}, with ${CC_RPI_GCC} and ${CC_RPI_GXX})")
   #in case of cross compiling, need to append the toolchain file to ExternalProject
   list(APPEND CURLPP_BUILD_ARGS "-DCC_RPI_ROOT=${CC_RPI_ROOT}")
   list(APPEND CURLPP_BUILD_ARGS "-DCC_RPI_GCC=${CC_RPI_GCC}")
   list(APPEND CURLPP_BUILD_ARGS "-DCC_RPI_GXX=${CC_RPI_GXX}")
   list(APPEND CURLPP_BUILD_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
endif(CMAKE_TOOLCHAIN_FILE)

ExternalProject_Add (curlpp_build
   DEPENDS           curl_build
   GIT_REPOSITORY    https://github.com/jpbarrette/curlpp.git
   GIT_TAG           5e25a51e8dc75e05b707e08536abee28e0a45738
   GIT_SHALLOW       ON
   GIT_CONFIG        advice.detachedHead=false
   PREFIX            ${CURLPP_ROOT}
   CMAKE_ARGS        ${CURLPP_BUILD_ARGS}
   INSTALL_COMMAND   ""
   BUILD_ALWAYS      OFF
)

SET_PROPERTY(TARGET curlpp_build PROPERTY FOLDER "External Libs")

ExternalProject_Get_Property(curlpp_build INSTALL_DIR)
set(CURLPP_INCLUDE_DIR ${INSTALL_DIR}/src/curlpp_build/include PARENT_SCOPE)

if(WIN32)
   set(CURLPP_LIBRARY ${INSTALL_DIR}/src/curlpp_build-build/$<CONFIG>/libcurlpp.lib PARENT_SCOPE)
else()
   set(CURLPP_LIBRARY ${INSTALL_DIR}/src/curlpp_build-build/libcurlpp.a PARENT_SCOPE)
endif()
