#include "stdafx.h"
#include "ConfigurationAdapter.h"
#include "tools/Log.h"
#include "AdapterHelpers.hpp"

CConfigurationAdapter::CConfigurationAdapter()
{
}

CConfigurationAdapter::~CConfigurationAdapter()
{
}



bool CConfigurationAdapter::adapt(int column, char** columValues, char** columnNames)
{
   bool returnValue = false;
   if(column == 0 || columValues == NULL || columnNames == NULL)
   {
      YADOMS_LOG(warning) << "Invalid arguments";
      returnValue = false;
   }
   else
   {
      boost::shared_ptr<CConfiguration> newConfiguration(new CConfiguration());

      for(int i=0; i<column ; i++)
      {
         MAP_COLMUN_IF(columnNames[i], "section", newConfiguration, Section)
         else MAP_COLMUN_IF(columnNames[i], "name", newConfiguration, Name)
         else MAP_COLMUN_IF(columnNames[i], "value", newConfiguration, Value)
         else MAP_COLMUN_IF(columnNames[i], "default_value", newConfiguration, DefaultValue)
         else MAP_COLMUN_IF(columnNames[i], "description", newConfiguration, Description)
         else MAP_COLMUN_ADAPTER_IF(columnNames[i], "last_modification_date", newConfiguration, LastModificationDate, boost::gregorian::from_string)
         else
         {
            //ignore it
            YADOMS_LOG(warning) << "Unknown column Name= " << columnNames[i] << " Value=" << columValues[i];
         }
      }
      m_results.push_back(newConfiguration);
      returnValue = true;
   }
   return returnValue;
}

std::vector<boost::shared_ptr<CConfiguration> > CConfigurationAdapter::getResults()  
{ 
   return m_results; 
}
