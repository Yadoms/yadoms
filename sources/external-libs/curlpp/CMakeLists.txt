# As far as the pull-request (https://github.com/jpbarrette/curlpp/pull/97) is not accepted into cURLpp,
# use cURLpp from https://github.com/Yadoms/curlpp with branch OptionToDisableSharedBuild.


set(CURL_INCLUDE_DIR ${CURL_SOURCE_DIR}/include)
set(CURL_LIBRARY libcurl)
set(CURLPP_BUILD_SHARED_LIBS OFF)

add_subdirectory(curlpp-0.8.1)

get_target_property(CURLPP_INCLUDE_DIRECTORIES curlpp_static INCLUDE_DIRECTORIES)
set(CURLPP_INCLUDE_DIRECTORIES ${CURLPP_INCLUDE_DIRECTORIES} PARENT_SCOPE)

get_target_property(CURLPP_LIBRARY_DIRECTORY curlpp_static TARGET_NAME)
set(CURLPP_LIBRARY_DIRECTORY ${CURLPP_LIBRARY_DIRECTORY} PARENT_SCOPE)

SET_PROPERTY(TARGET curlpp_static PROPERTY FOLDER "External Libs")
#TODO  source_group("External Libs\\curlpp"  "external-libs/curlpp/curlpp-0.8.1/*.*")
