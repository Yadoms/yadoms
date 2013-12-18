#include "stdafx.h"
#include "HardwareAdapter.h"
#include "tools/Log.h"
#include "AdapterHelpers.hpp"
#include "database/sqlite/SQLiteDatabaseTables.h"


CHardwareAdapter::CHardwareAdapter()
{
}

CHardwareAdapter::~CHardwareAdapter()
{
}


bool CHardwareAdapter::adapt(int column, char** columValues, char** columnNames)
{
   ADAPT(Hardware,
         ((Id)(INT_ADAPTER)(0))
         ((Name)(STRING_ADAPTER)(""))
         ((PluginName)(STRING_ADAPTER)(""))
         ((Configuration)(STRING_ADAPTER)(""))
         ((Enabled)(BOOL_ADAPTER)(true))
         ((Deleted)(BOOL_ADAPTER)(false))
   )
}

std::vector<boost::shared_ptr<CHardware> > CHardwareAdapter::getResults()  
{ 
   return m_results; 
}

