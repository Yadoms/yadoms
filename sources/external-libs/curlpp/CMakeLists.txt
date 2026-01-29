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

   #provide OSXCROSS vars
   set(CURL_PATHCMD)
   if(OSXCROSS_HOST)
      list(APPEND CURLPP_BUILD_ARGS "-DOSXCROSS_HOST=${OSXCROSS_HOST}")
      list(APPEND CURLPP_BUILD_ARGS "-DCMAKE_C_COMPILER=${OSXCROSS_HOST}-clang")
      list(APPEND CURLPP_BUILD_ARGS "-DCMAKE_CXX_COMPILER=${OSXCROSS_HOST}-clang++")
      set(CURLPP_PATHCMD ${OSXCROSS_HOST}-osxcross-conf)      
   endif()
   
   if(OSXCROSS_TARGET_DIR)
      list(APPEND CURLPP_BUILD_ARGS "-DOSXCROSS_TARGET_DIR=${OSXCROSS_TARGET_DIR}")
   endif()
   if(OSXCROSS_TARGET)
      list(APPEND CURLPP_BUILD_ARGS "-DOSXCROSS_TARGET=${OSXCROSS_TARGET}")
   endif()
   if(OSXCROSS_SDK)
      list(APPEND CURLPP_BUILD_ARGS "-DOSXCROSS_SDK=${OSXCROSS_SDK}")
   endif()


   #provide RPI_ROOT and also GCC and G++ (used by toolchain file)
   if(CC_RPI_ROOT)
      list(APPEND CURLPP_BUILD_ARGS "-DCC_RPI_ROOT=${CC_RPI_ROOT}")
   endif()
   
   if(CC_RPI_GCC)
      list(APPEND CURLPP_BUILD_ARGS "-DCC_RPI_GCC=${CC_RPI_GCC}")
   endif()
  
   if(CC_RPI_GXX)
      list(APPEND CURLPP_BUILD_ARGS "-DCC_RPI_GXX=${CC_RPI_GXX}")
   endif()
   
   #append all potentials dependencies that external project may need (openssl mostly)
   if(OPENSSL_ROOT)
      list(APPEND CURLPP_BUILD_ARGS "-DOPENSSL_ROOT_DIR=${OPENSSL_ROOT}")
   endif()
   if(PROTOBUF_ROOT)
      list(APPEND CURLPP_BUILD_ARGS "-DPROTOBUF_ROOT=${PROTOBUF_ROOT}")
   endif()
   if(SWIG_ROOT)
      list(APPEND CURLPP_BUILD_ARGS "-DSWIG_ROOT=${SWIG_ROOT}")
   endif()   
   if(Python2_ROOT)
      list(APPEND CURLPP_BUILD_ARGS "-DPython2_ROOT=${Python2_ROOT}")
   endif()  
   if(Python3_ROOT)
      list(APPEND CURLPP_BUILD_ARGS "-DPython3_ROOT=${Python3_ROOT}")
   endif()  
   
   list(APPEND CURLPP_BUILD_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
   
   
endif(CMAKE_TOOLCHAIN_FILE)

ExternalProject_Add (curlpp_build
   DEPENDS           curl_build
   GIT_REPOSITORY    https://github.com/jpbarrette/curlpp.git
   GIT_TAG           d080e301ec71061c46049d23fedfb8106be3153f
   GIT_CONFIG        advice.detachedHead=false
   PATCH_COMMAND     ${CURLPP_PATHCMD}   
   PREFIX            ${CURLPP_ROOT}
   CMAKE_ARGS        ${CURLPP_BUILD_ARGS}
   UPDATE_COMMAND    ""
   INSTALL_COMMAND   ""
)

SET_PROPERTY(TARGET curlpp_build PROPERTY FOLDER "External Libs")

ExternalProject_Get_Property(curlpp_build INSTALL_DIR)
set(CURLPP_INCLUDE_DIR ${INSTALL_DIR}/src/curlpp_build/include PARENT_SCOPE)

if(WIN32)
   set(CURLPP_LIBRARY ${INSTALL_DIR}/src/curlpp_build-build/$<CONFIG>/libcurlpp.lib PARENT_SCOPE)
else()
   set(CURLPP_LIBRARY ${INSTALL_DIR}/src/curlpp_build-build/libcurlpp.a PARENT_SCOPE)
endif()
