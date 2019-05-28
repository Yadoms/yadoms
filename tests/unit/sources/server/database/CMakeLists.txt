IF(NOT DISABLE_TEST_ENUM)
   
   ADD_YADOMS_SOURCES(
		server/database/entities/Entities.h
		server/database/entities/Entities.cpp
		server/database/common/Query.h
		server/database/common/Query.cpp
		server/database/common/QuerySpecializations.h
		server/database/common/DatabaseColumn.h
		server/database/common/DatabaseColumn.cpp
		server/database/common/DatabaseTables.h
		server/database/common/DatabaseTables.cpp
		server/database/pgsql/PgsqlQuery.h
		server/database/pgsql/PgsqlQuery.cpp
		server/database/sqlite/SQLiteQuery.h
		server/database/sqlite/SQLiteQuery.cpp
	)
	
   ADD_SOURCES(TestEnum.cpp)
   ADD_SOURCES(TestQuery.cpp)

ENDIF()

set (YADOMS_TESTS_SRC ${YADOMS_TESTS_SRC} PARENT_SCOPE)
