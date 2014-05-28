#pragma once

#include "SQLite3Extension.hpp"
#include "AdapterHelpers.hpp"
#include "ISQLiteResultAdapter.h"
#include "database/sqlite/SQLiteDatabaseTables.h"


namespace database { 
namespace sqlite { 
namespace adapters { 


   //---------------------------------
   ///\brief Declare all adapter classes
   //---------------------------------
   DECLARE_ADAPTER_HEADER(Plugin)
   DECLARE_ADAPTER_HEADER(Configuration)
   DECLARE_ADAPTER_HEADER(Keyword)
   DECLARE_ADAPTER_HEADER(Page)
   DECLARE_ADAPTER_HEADER(Widget)
   DECLARE_ADAPTER_HEADER(PluginEventLogger)
   DECLARE_ADAPTER_HEADER(EventLogger)
   DECLARE_ADAPTER_HEADER(Message)
   DECLARE_ADAPTER_HEADER(MessageContent)
   DECLARE_ADAPTER_HEADER(Capacity)
   DECLARE_ADAPTER_HEADER(InterDeviceCapacity)
   DECLARE_ADAPTER_HEADER(Device)
   
} //namespace adapters
} //namespace sqlite
} //namespace database 

