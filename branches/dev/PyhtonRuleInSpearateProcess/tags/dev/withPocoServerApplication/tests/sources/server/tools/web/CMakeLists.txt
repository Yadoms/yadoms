IF(NOT DISABLE_TEST_TOOLS_WEB)
   
   ADD_YADOMS_SOURCES(
		server/tools/tools/web/UriSinglePatternValidator.h
		server/tools/tools/web/UriSinglePatternValidator.cpp
	)

   ADD_SOURCES(TestUriValidator.cpp)
   
ENDIF()

set (YADOMS_TESTS_SRC ${YADOMS_TESTS_SRC} PARENT_SCOPE)
