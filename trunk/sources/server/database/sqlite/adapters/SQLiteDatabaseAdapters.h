#pragma once

#include "SQLite3Extension.hpp"
#include "AdapterHelpers.hpp"
#include "ISQLiteResultAdapter.h"
#include "database/sqlite/SQLiteDatabaseTables.h"

//---------------------------------
///\brief Declare all adapter classes
//---------------------------------
DECLARE_ADAPTER_HEADER(Hardware)
DECLARE_ADAPTER_HEADER(Configuration)
DECLARE_ADAPTER_HEADER(Acquisition)
DECLARE_ADAPTER_HEADER(Device)
DECLARE_ADAPTER_HEADER(Keyword)
DECLARE_ADAPTER_HEADER(Page)
DECLARE_ADAPTER_HEADER(Widget)
