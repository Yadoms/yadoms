#include "stdafx.h"
#include "SQLiteDatabaseTables.h"
#include "SQLiteDatabaseTablesHelpers.hpp"

DECLARE_TABLE_COLUMN_NAMES(Hardware, 
   ((Id)("id"))
   ((Name)("name"))
   ((PluginName)("pluginName"))
   ((Configuration)("configuration"))
   ((Enabled)("enabled"))
   ((Deleted)("deleted"))
)

DECLARE_TABLE_COLUMN_NAMES(Configuration, 
   ((Section)("section"))
   ((Name)("name"))
   ((Value)("value"))
   ((DefaultValue)("default_value"))
   ((Description)("description"))
   ((LastModificationDate)("last_modification_date"))
)
