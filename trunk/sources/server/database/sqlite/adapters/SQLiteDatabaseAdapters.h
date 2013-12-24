#pragma once

#include "AdapterHelpers.hpp"
#include "ISQLiteResultAdapter.h"
#include "database/sqlite/SQLiteDatabaseTables.h"


DECLARE_ADAPTER(Hardware,
      ((Id)(INT_ADAPTER)(0))
      ((Name)(STRING_ADAPTER)(""))
      ((PluginName)(STRING_ADAPTER)(""))
      ((Configuration)(STRING_ADAPTER)(""))
      ((Enabled)(BOOL_ADAPTER)(true))
      ((Deleted)(BOOL_ADAPTER)(false))
)

DECLARE_ADAPTER(Configuration,
   ((Section)(STRING_ADAPTER)(""))
   ((Name)(STRING_ADAPTER)(""))
   ((Value)(STRING_ADAPTER)(""))
   ((DefaultValue)(STRING_ADAPTER)(""))
   ((Description)(STRING_ADAPTER)(""))
   ((LastModificationDate)(DATE_ADAPTER)(boost::gregorian::day_clock::local_day()))
)

DECLARE_ADAPTER(Acquisition,
   ((Id)(INT_ADAPTER)(0))
   ((Source)(STRING_ADAPTER)(""))
   ((Keyword)(STRING_ADAPTER)(""))
   ((Value)(STRING_ADAPTER)(""))
   ((Date)(DATE_ADAPTER)(boost::gregorian::day_clock::local_day()))
)

DECLARE_ADAPTER(Device,
   ((Id)(INT_ADAPTER)(0))
   ((DataSource)(STRING_ADAPTER)(""))
   ((Name)(STRING_ADAPTER)(""))
   ((Configuration)(STRING_ADAPTER)(""))
)

DECLARE_ADAPTER(Keyword,
   ((Name)(STRING_ADAPTER)(""))
)
