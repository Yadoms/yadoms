#pragma once

#include "database/common/DatabaseTablesHelpers.hpp"


namespace database { 
namespace pgsql { 

   DECLARE_TABLE(PgsqlTables, 
      (Database)
      (Schema)
      (Table)
      (TableType)
      (SelfColumn)
      (Generation)
      (DatabaseUserType)
      (SchemaUserType)
      (TableUserType)
      (IsInsertable)
      (IsTyped)
      (CommitAction)
   )

} //namespace pgsql
} //namespace database 

