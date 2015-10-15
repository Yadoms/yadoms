IF(NOT DISABLE_TEST_ENUM)
   
   ADD_YADOMS_SOURCES(
		server/database/entities/Entities.h
		server/database/entities/Entities.cpp
	)
	
   ADD_SOURCES(TestEnum.cpp)
   
ENDIF()

set (YADOMS_TESTS_SRC ${YADOMS_TESTS_SRC} PARENT_SCOPE)
