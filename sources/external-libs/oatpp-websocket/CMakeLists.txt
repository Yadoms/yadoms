include(ExternalProject)

set(OATPP_WEBSOCKET_ROOT ${CMAKE_BINARY_DIR}/external-libs/oatpp-websocket)

message("OATPP_ROOT=${OATPP_ROOT}")
message("OATPP_LIBRARY=${OATPP_LIBRARY}")
get_filename_component(OATPP_LIBRARY_DIR ${OATPP_LIBRARY} DIRECTORY)
message("OATPP_LIBRARY_DIR=${OATPP_LIBRARY_DIR}")

list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOATPP_MODULES_LOCATION=CUSTOM")
list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOATPP_DIR_SRC=${OATPP_ROOT}/src/oatpp_build/src")
list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOATPP_DIR_LIB=${OATPP_LIBRARY}")

list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOATPP_BUILD_TESTS=OFF")
list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOATPP_INSTALL=OFF")
if(NOT WIN32)
   list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DCMAKE_POSITION_INDEPENDENT_CODE=ON")
endif()


if(CMAKE_TOOLCHAIN_FILE)
message("OATPP-WEBSOCKET : cross compilation (toolchain:${CMAKE_TOOLCHAIN_FILE}, with ${CC_RPI_GCC} and ${CC_RPI_GXX})")

   #provide OSXCROSS vars
   if(OSXCROSS_HOST)
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOSXCROSS_HOST=${OSXCROSS_HOST}")
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DCMAKE_C_COMPILER=${OSXCROSS_HOST}-clang")
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DCMAKE_CXX_COMPILER=${OSXCROSS_HOST}-clang++")
      set(OATPP_WEBSOCKET_PATCH_CMD ${OSXCROSS_HOST}-osxcross-conf)      
   endif()
   
   if(OSXCROSS_TARGET_DIR)
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOSXCROSS_TARGET_DIR=${OSXCROSS_TARGET_DIR}")
   endif()
   if(OSXCROSS_TARGET)
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOSXCROSS_TARGET=${OSXCROSS_TARGET}")
   endif()
   if(OSXCROSS_SDK)
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOSXCROSS_SDK=${OSXCROSS_SDK}")
   endif()


   #provide RPI_ROOT and also GCC and G++ (used by toolchain file)
   if(CC_RPI_ROOT)
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DCC_RPI_ROOT=${CC_RPI_ROOT}")
   endif()
   
   if(CC_RPI_GCC)
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DCC_RPI_GCC=${CC_RPI_GCC}")
   endif()
  
   if(CC_RPI_GXX)
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DCC_RPI_GXX=${CC_RPI_GXX}")
   endif()
   
   #append all potentials dependencies that external project may need (openssl mostly)
   if(OPENSSL_ROOT)
      list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DOPENSSL_ROOT_DIR=${OPENSSL_ROOT}")
   endif()  
   
   list(APPEND OATPP_WEBSOCKET_BUILD_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
   
   
endif(CMAKE_TOOLCHAIN_FILE)

ExternalProject_Add (oatpp-websocket_build
   DEPENDS           oatpp_build
   GIT_REPOSITORY    https://github.com/yadoms/oatpp-websocket.git
   GIT_TAG           b9baf4dcac1fb0ba3bba6a8c355e8817fd0b4ca6
   GIT_SHALLOW       ON
   GIT_CONFIG        advice.detachedHead=false
   PATCH_COMMAND     ${OATPP_WEBSOCKET_PATCH_CMD}   
   PREFIX            ${OATPP_WEBSOCKET_ROOT}
   CMAKE_ARGS        ${OATPP_WEBSOCKET_BUILD_ARGS}
   INSTALL_COMMAND   ""
   BUILD_ALWAYS      OFF
)

SET_PROPERTY(TARGET oatpp-websocket_build PROPERTY FOLDER "External Libs")

ExternalProject_Get_Property(oatpp-websocket_build INSTALL_DIR)
set(OATPP_WEBSOCKET_INCLUDE_DIR ${INSTALL_DIR}/src/oatpp-websocket_build/src PARENT_SCOPE)

if(WIN32)
   set(OATPP_WEBSOCKET_LIBRARY ${INSTALL_DIR}/src/oatpp-websocket_build-build/src/$<CONFIG>/oatpp-websocket.lib PARENT_SCOPE)
else()
   set(OATPP_WEBSOCKET_LIBRARY ${INSTALL_DIR}/src/oatpp-websocket_build-build/src/liboatpp-websocket.a PARENT_SCOPE)
endif()

