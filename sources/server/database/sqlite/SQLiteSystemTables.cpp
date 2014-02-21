#include "stdafx.h"
#include "SQLiteSystemTables.h"
#include "database/sqlite/SQLiteDatabaseTablesHelpers.hpp"

namespace server { 
namespace database { 
namespace sqlite { 

   DECLARE_TABLE_COLUMN_NAMES(SqliteMaster, "sqlite_master",
      ((Type)("type"))
      ((Name)("name"))
      ((TableName)("tbl_name"))
      ((RootPage)("rootpage"))
      ((Sql)("sql"))
   )

   DECLARE_STATIC_TABLE_CREATION_SCRIPT(SqliteMaster, "cannot create system tables")

} //namespace sqlite
} //namespace database 
} //namespace server
