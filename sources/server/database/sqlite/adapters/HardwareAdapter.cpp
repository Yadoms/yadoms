#include "stdafx.h"
#include "HardwareAdapter.h"
#include "tools/Log.h"
#include "AdapterHelpers.hpp"

CHardwareAdapter::CHardwareAdapter()
{
}

CHardwareAdapter::~CHardwareAdapter()
{
}

bool CHardwareAdapter::adapt(int column, char** columValues, char** columnNames)
{
   bool returnValue = false;
   if(column == 0 || columValues == NULL || columnNames == NULL)
   {
      YADOMS_LOG(warning) << "Invalid arguments";
      returnValue = false;
   }
   else
   {
      boost::shared_ptr<CHardware> newHardware(new CHardware());

      std::string columnId("id");
      std::string columnName("name");
      std::string columnPluginName("pluginName");
      std::string columnConfiguration("configuration");
      std::string columnEnabled("enabled");
      std::string columnDeleted("deleted");

      for(int i=0; i<column ; i++)
      {
         MAP_COLMUN_ADAPTER_IF(columnNames[i], "id", newHardware, Id, boost::lexical_cast<int>)
         else MAP_COLMUN_IF(columnNames[i], "name", newHardware, Name)
         else MAP_COLMUN_IF(columnNames[i], "pluginName", newHardware, PluginName)
         else MAP_COLMUN_IF(columnNames[i], "configuration", newHardware, Configuration)
         else MAP_COLMUN_ADAPTER_IF(columnNames[i], "enabled", newHardware, Enabled, boost::lexical_cast<bool>)
         else MAP_COLMUN_ADAPTER_IF(columnNames[i], "deleted", newHardware, Deleted, boost::lexical_cast<bool>)
         else
         {
            //ignore it
            YADOMS_LOG(warning) << "Unknown column Name= " << columnNames[i] << " Value=" << columValues[i];
         }
      }
      m_results.push_back(newHardware);
      returnValue = true;
   }
   return returnValue;
}

std::vector<boost::shared_ptr<CHardware> > CHardwareAdapter::getResults()  
{ 
   return m_results; 
}
