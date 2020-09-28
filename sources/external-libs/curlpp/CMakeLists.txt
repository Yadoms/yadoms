# cURLpp is available from https://github.com/jpbarrette/curlpp


set(CURL_INCLUDE_DIR ${CURL_SOURCE_DIR}/include)
set(CURL_LIBRARY libcurl)
set(CURLPP_BUILD_SHARED_LIBS OFF)

add_subdirectory(curlpp-0.8.1)

if(COTIRE_USE)
   if(COTIRE_USE_UNITY)
      set_target_properties(curlpp_static PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
   else()
      set_target_properties(curlpp_static PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
   endif()
   
   cotire(curlpp_static)
endif()

get_target_property(CURLPP_INCLUDE_DIRECTORIES curlpp_static INCLUDE_DIRECTORIES)
set(CURLPP_INCLUDE_DIRECTORIES ${CURLPP_INCLUDE_DIRECTORIES} PARENT_SCOPE)

get_target_property(CURLPP_LIBRARY_DIRECTORY curlpp_static TARGET_NAME)
set(CURLPP_LIBRARY_DIRECTORY ${CURLPP_LIBRARY_DIRECTORY} PARENT_SCOPE)

SET_PROPERTY(TARGET curlpp_static PROPERTY FOLDER "External Libs")
