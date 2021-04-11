include(ExternalProject)

   set(CURLPP_ROOT ${CMAKE_BINARY_DIR}/external-libs/curlpp)

   list(APPEND CURLPP_BUILD_ARGS "-DCURL_INCLUDE_DIR=${CURL_SOURCE_DIR}/include")
   list(APPEND CURLPP_BUILD_ARGS "-DCURL_LIBRARY=libcurl")
   list(APPEND CURLPP_BUILD_ARGS "-DCURLPP_BUILD_SHARED_LIBS=OFF")
   list(APPEND CURLPP_BUILD_ARGS "-DCURLPP_STATIC_CRT=ON")

   ExternalProject_Add (curlpp_build
      GIT_REPOSITORY    https://github.com/jpbarrette/curlpp.git
      GIT_TAG           origin/master
      GIT_SHALLOW       ON
      GIT_CONFIG        advice.detachedHead=false
      PREFIX            ${CURLPP_ROOT}
      CMAKE_ARGS        ${CURLPP_BUILD_ARGS}
      INSTALL_COMMAND   ""
      INSTALL_DIR       ${CURLPP_ROOT}
      BUILD_ALWAYS      OFF
   )

SET_PROPERTY(TARGET curlpp_build PROPERTY FOLDER "External Libs")

ExternalProject_Get_Property(curlpp_build INSTALL_DIR)
set(CURLPP_INCLUDE_DIRECTORIES ${INSTALL_DIR}/src/curlpp_build/include PARENT_SCOPE)
set(CURLPP_LIBRARY ${INSTALL_DIR}/src/curlpp_build-build/$<CONFIG>/libcurlpp.lib PARENT_SCOPE)
