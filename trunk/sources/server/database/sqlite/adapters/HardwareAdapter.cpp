#include "stdafx.h"
#include "HardwareAdapter.h"
#include "boost/lexical_cast.hpp"

CHardwareAdapter::CHardwareAdapter()
{
}

CHardwareAdapter::~CHardwareAdapter()
{
}

bool CHardwareAdapter::adapt(int column, char** columValues, char** columnName)
{
   bool returnValue = false;
   if(column == 0 || columValues == NULL || columnName == NULL)
   {
      BOOST_LOG_TRIVIAL(warning) << "Invalid arguments";
      returnValue = false;
   }
   else
   {
      CHardware newHardware;
	
	std::string columnId("id");
	std::string columnName("name");
	std::string columnPluginName("pluginName");

      for(int i=0; i<column ; i++)
      {

         if(columnId.compare((const char*)columnName[i]) == 0)
            newHardware.setId( boost::lexical_cast<int>(columValues[i]) );
         else if(columnName.compare((const char*)columnName[i]) == 0)
            newHardware.setName( columValues[i] );
         else if(columnPluginName.compare((const char*)columnName[i]) == 0)
            newHardware.setPluginName( columValues[i] );
         else
         {
            //ignore it
            BOOST_LOG_TRIVIAL(warning) << "Unknown column Name= " << columnName[i] << " Value=" << columValues[i];
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
