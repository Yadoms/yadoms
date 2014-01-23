#include "stdafx.h"
#include "SQLiteDatabaseTables.h"
#include "SQLiteDatabaseTablesHelpers.hpp"

DECLARE_TABLE_COLUMN_NAMES(Hardware, "Hardware",
   ((Id)("id"))
   ((Name)("name"))
   ((PluginName)("pluginName"))
   ((Configuration)("configuration"))
   ((Enabled)("enabled"))
   ((Deleted)("deleted"))
)

DECLARE_TABLE_COLUMN_NAMES(Configuration,  "Configuration",
   ((Section)("section"))
   ((Name)("name"))
   ((Value)("value"))
   ((DefaultValue)("default_value"))
   ((Description)("description"))
   ((LastModificationDate)("last_modification_date"))
)

DECLARE_TABLE_COLUMN_NAMES(Acquisition,  "Acquisition",
   ((Id)("id"))
   ((Source)("source"))
   ((Keyword)("keyword"))
   ((Value)("value"))
   ((Date)("date"))
)


DECLARE_TABLE_COLUMN_NAMES(Device,  "Device",
   ((Id)("id"))
   ((DataSource)("data_source"))
   ((Name)("name"))
   ((Configuration)("configuration"))
)


DECLARE_TABLE_COLUMN_NAMES(Keyword,  "Keyword",
   ((Name)("name"))
)

DECLARE_TABLE_COLUMN_NAMES(Page,  "Page",
   ((Id)("id"))
   ((Name)("name"))
)

DECLARE_TABLE_COLUMN_NAMES(Widget,  "Widget",
   ((Id)("id"))
   ((IdPage)("idPage"))
   ((Path)("path"))
   ((SizeX)("sizeX"))
   ((SizeY)("sizeY"))
   ((Configuration)("configuration"))
)

