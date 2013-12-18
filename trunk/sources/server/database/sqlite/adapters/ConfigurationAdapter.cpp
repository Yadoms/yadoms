#include "stdafx.h"
#include "ConfigurationAdapter.h"
#include "tools/Log.h"
#include "AdapterHelpers.hpp"
#include "database/sqlite/SQLiteDatabaseTables.h"

CConfigurationAdapter::CConfigurationAdapter()
{
}

CConfigurationAdapter::~CConfigurationAdapter()
{
}



bool CConfigurationAdapter::adapt(int column, char** columValues, char** columnNames)
{
   ADAPT(Configuration,
      ((Section)(STRING_ADAPTER)(""))
      ((Name)(STRING_ADAPTER)(""))
      ((Value)(STRING_ADAPTER)(""))
      ((DefaultValue)(STRING_ADAPTER)(""))
      ((Description)(STRING_ADAPTER)(""))
      ((LastModificationDate)(DATE_ADAPTER)(boost::gregorian::day_clock::local_day()))
   )

}

std::vector<boost::shared_ptr<CConfiguration> > CConfigurationAdapter::getResults()  
{ 
   return m_results; 
}
