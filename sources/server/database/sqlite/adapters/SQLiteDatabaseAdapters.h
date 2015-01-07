#pragma once

#include "SQLite3Extension.hpp"
#include "AdapterHelpers.hpp"
#include "ISQLiteResultAdapter.h"
#include "database/sqlite/SQLiteDatabaseTables.h"

//include specific adapters
#include "RecipientFieldsAdapter.h"

namespace database { 
namespace sqlite { 
namespace adapters { 


   //---------------------------------
   ///\brief Declare all adapter classes
   //---------------------------------
   DECLARE_ADAPTER_HEADER(Plugin)
   DECLARE_ADAPTER_HEADER(Configuration)
   DECLARE_ADAPTER_HEADER(Page)
   DECLARE_ADAPTER_HEADER(Widget)
   DECLARE_ADAPTER_HEADER(PluginEventLogger)
   DECLARE_ADAPTER_HEADER(EventLogger)
   DECLARE_ADAPTER_HEADER(Device)
   DECLARE_ADAPTER_HEADER(Keyword)
   DECLARE_ADAPTER_HEADER(Acquisition)
   DECLARE_ADAPTER_HEADER(AcquisitionSummary)
   DECLARE_ADAPTER_HEADER(Rule)
	DECLARE_ADAPTER_HEADER(Recipient)
	DECLARE_ADAPTER_HEADER(Field)
//   DECLARE_ADAPTER_HEADER(Contact)
  
} //namespace adapters
} //namespace sqlite
} //namespace database 

