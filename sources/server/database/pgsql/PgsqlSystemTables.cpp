#include "stdafx.h"
#include "PgsqlSystemTables.h"


namespace database
{
   namespace pgsql
   {
      DECLARE_TABLE_COLUMN_NAMES(PgsqlTables, "information_schema.tables",
         ((Database)("table_catalog"))
         ((Schema)("table_schema"))
         ((Table)("table_name"))
         ((TableType)("table_type"))
         ((SelfColumn)("self_referencing_column_name"))
         ((Generation)("reference_generation"))
         ((DatabaseUserType)("user_defined_type_catalog"))
         ((SchemaUserType)("user_defined_type_schema"))
         ((TableUserType)("user_defined_type_name"))
         ((IsInsertable)("is_insertable_into"))
         ((IsTyped)("is_typed"))
         ((CommitAction)("commit_action"))
      )

      DECLARE_TABLE_COLUMN_NAMES(PgDatabase, "pg_catalog.pg_database",
         ((DatabaseName)("datname"))
      )
   } //namespace pgsql
} //namespace database 


