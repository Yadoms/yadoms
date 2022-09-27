include(ExternalProject)

set(OATPP_ROOT ${CMAKE_BINARY_DIR}/external-libs/oatpp)

list(APPEND OATPP_BUILD_ARGS "-DOATPP_BUILD_TESTS=OFF")
list(APPEND OATPP_BUILD_ARGS "-DOATPP_LINK_TEST_LIBRARY=OFF")
list(APPEND OATPP_BUILD_ARGS "-DOATPP_INSTALL=OFF")
list(APPEND OATPP_BUILD_ARGS "-DOATPP_MSVC_LINK_STATIC_RUNTIME=ON")
if(NOT WIN32)
   list(APPEND OATPP_BUILD_ARGS "-DCMAKE_POSITION_INDEPENDENT_CODE=ON")
endif()


if(CMAKE_TOOLCHAIN_FILE)
message("OATPP : cross compilation (toolchain:${CMAKE_TOOLCHAIN_FILE}, with ${CC_RPI_GCC} and ${CC_RPI_GXX})")

   #provide OSXCROSS vars
   if(OSXCROSS_HOST)
      list(APPEND OATPP_BUILD_ARGS "-DOSXCROSS_HOST=${OSXCROSS_HOST}")
      list(APPEND OATPP_BUILD_ARGS "-DCMAKE_C_COMPILER=${OSXCROSS_HOST}-clang")
      list(APPEND OATPP_BUILD_ARGS "-DCMAKE_CXX_COMPILER=${OSXCROSS_HOST}-clang++")
      set(OATPP_PATCH_CMD ${OSXCROSS_HOST}-osxcross-conf)      
   endif()
   
   if(OSXCROSS_TARGET_DIR)
      list(APPEND OATPP_BUILD_ARGS "-DOSXCROSS_TARGET_DIR=${OSXCROSS_TARGET_DIR}")
   endif()
   if(OSXCROSS_TARGET)
      list(APPEND OATPP_BUILD_ARGS "-DOSXCROSS_TARGET=${OSXCROSS_TARGET}")
   endif()
   if(OSXCROSS_SDK)
      list(APPEND OATPP_BUILD_ARGS "-DOSXCROSS_SDK=${OSXCROSS_SDK}")
   endif()


   #provide RPI_ROOT and also GCC and G++ (used by toolchain file)
   if(CC_RPI_ROOT)
      list(APPEND OATPP_BUILD_ARGS "-DCC_RPI_ROOT=${CC_RPI_ROOT}")
   endif()
   
   if(CC_RPI_GCC)
      list(APPEND OATPP_BUILD_ARGS "-DCC_RPI_GCC=${CC_RPI_GCC}")
   endif()
  
   if(CC_RPI_GXX)
      list(APPEND OATPP_BUILD_ARGS "-DCC_RPI_GXX=${CC_RPI_GXX}")
   endif()
   
   #append all potentials dependencies that external project may need (openssl mostly)
   if(OPENSSL_ROOT)
      list(APPEND OATPP_BUILD_ARGS "-DOPENSSL_ROOT_DIR=${OPENSSL_ROOT}")
   endif()  
   
   list(APPEND OATPP_BUILD_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
   
   
endif(CMAKE_TOOLCHAIN_FILE)

ExternalProject_Add (oatpp_build
   GIT_REPOSITORY    https://github.com/oatpp/oatpp.git
   GIT_TAG           306a01c1e2128859ff6fa8422c788eca43a1fb3c
   GIT_CONFIG        advice.detachedHead=false
   PATCH_COMMAND     ${OATPP_PATCH_CMD}   
   PREFIX            ${OATPP_ROOT}
   CMAKE_ARGS        ${OATPP_BUILD_ARGS}
   UPDATE_COMMAND    ""
   INSTALL_COMMAND   ""
)

SET_PROPERTY(TARGET oatpp_build PROPERTY FOLDER "External Libs")

ExternalProject_Get_Property(oatpp_build INSTALL_DIR)
set(OATPP_INCLUDE_DIR ${INSTALL_DIR}/src/oatpp_build/src PARENT_SCOPE)

if(WIN32)
   set(OATPP_LIBRARY ${INSTALL_DIR}/src/oatpp_build-build/src/$<CONFIG>/oatpp.lib PARENT_SCOPE)
else()
   set(OATPP_LIBRARY ${INSTALL_DIR}/src/oatpp_build-build/src/liboatpp.a PARENT_SCOPE)
endif()

set(OATPP_ROOT ${OATPP_ROOT} PARENT_SCOPE)
