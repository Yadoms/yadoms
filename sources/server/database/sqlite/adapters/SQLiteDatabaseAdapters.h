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
   //TODO normalement ils devraient tous pouvoir passer en DECLARE_CONST_ADAPTER_HEADER, sauf que la fonction CDataContainer::set avec vector en paramètre ne le supporte pas toujours, à creuser
   //TODO si corrigé, alors renommer DECLARE_CONST_ADAPTER_HEADER en DECLARE_ADAPTER_HEADER
   DECLARE_ADAPTER_HEADER(Plugin)
   DECLARE_ADAPTER_HEADER(Configuration)
   DECLARE_ADAPTER_HEADER(Page)
   DECLARE_ADAPTER_HEADER(Widget)
   DECLARE_ADAPTER_HEADER(PluginEventLogger)
   DECLARE_ADAPTER_HEADER(EventLogger)
   DECLARE_CONST_ADAPTER_HEADER(Device)
   DECLARE_CONST_ADAPTER_HEADER(Keyword)
   DECLARE_ADAPTER_HEADER(Acquisition)
   DECLARE_CONST_ADAPTER_HEADER(AcquisitionSummary)
   DECLARE_CONST_ADAPTER_HEADER(Rule)
	DECLARE_CONST_ADAPTER_HEADER(Recipient)
	DECLARE_CONST_ADAPTER_HEADER(Field)
//   DECLARE_CONST_ADAPTER_HEADER(Contact)
  
} //namespace adapters
} //namespace sqlite
} //namespace database 

