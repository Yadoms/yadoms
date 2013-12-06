#include "stdafx.h"
#include "HardwareAdapter.h"
#include "boost/lexical_cast.hpp"
#include "tools/Log.h"

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
      CHardware newHardware;

      std::string columnId("id");
      std::string columnName("name");
      std::string columnPluginName("pluginName");
      std::string columnConfiguration("configuration");

      for(int i=0; i<column ; i++)
      {
         std::string colName(columnNames[i]);

         if(boost::iequals(columnId, colName))
            newHardware.setId( boost::lexical_cast<int>(columValues[i]) );
         else if(boost::iequals(columnName, colName))
            newHardware.setName( columValues[i] );
         else if(boost::iequals(columnPluginName, colName))
            newHardware.setPluginName( columValues[i] );
         else if(boost::iequals(columnConfiguration, colName))
            newHardware.setConfiguration( columValues[i] );
         else
         {
            //ignore it
            YADOMS_LOG(warning) << "Unknown column Name= " << columnName[i] << " Value=" << columValues[i];
         }
      }
      m_results.push_back(newHardware);
      returnValue = true;
   }
   return returnValue;
}

std::vector<CHardware> CHardwareAdapter::getResults()  
{ 
   return m_results; 
}
