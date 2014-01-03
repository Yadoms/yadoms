#pragma once

#include "database/sqlite/SQLiteDatabaseTablesHelpers.hpp"

#define SQLITEMASTER_TABLE "table"

DECLARE_TABLE(SqliteMaster, 
   (Type)
   (Name)
   (TableName)
   (RootPage)
   (Sql)
)

