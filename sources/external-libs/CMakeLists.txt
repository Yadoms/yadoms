#place here all libraries

#SQLite
IF(USE_SQLITE_DATABASE)
	add_subdirectory(SQLite)
	set(SQLITE_INCLUDE_DIRECTORY ${SQLITE_INCLUDE_DIRECTORY} PARENT_SCOPE)
ENDIF()

#curl
add_subdirectory(curl)
set(CURL_INCLUDE_DIR ${CURL_INCLUDE_DIR} PARENT_SCOPE)
set(CURL_LIBRARY ${CURL_LIBRARY} PARENT_SCOPE)

#curlpp
add_subdirectory(curlpp)
set(CURLPP_INCLUDE_DIR ${CURLPP_INCLUDE_DIR} PARENT_SCOPE)
set(CURLPP_LIBRARY ${CURLPP_LIBRARY} PARENT_SCOPE)
