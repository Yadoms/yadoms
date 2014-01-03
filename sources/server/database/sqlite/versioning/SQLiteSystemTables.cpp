#include "stdafx.h"
#include "SQLiteSystemTables.h"
#include "database/sqlite/SQLiteDatabaseTablesHelpers.hpp"

DECLARE_TABLE_COLUMN_NAMES(SqliteMaster, "sqlite_master",
   ((Type)("type"))
   ((Name)("name"))
   ((TableName)("tbl_name"))
   ((RootPage)("rootpage"))
   ((Sql)("sql"))
)

