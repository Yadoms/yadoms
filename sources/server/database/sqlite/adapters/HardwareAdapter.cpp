#include "HardwareAdapter.h"
#include "boost/lexical_cast.hpp"
#include "boost/log/trivial.hpp"

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

      for(int i=0; i<column ; i++)
      {
         if(_strnicmp("id", columnName[i], 2) == 0)
            newHardware.setId( boost::lexical_cast<int>(columValues[i]) );
         else if(_strnicmp("name", columnName[i], 4) == 0)
            newHardware.setName( columValues[i] );
         else if(_strnicmp("pluginName", columnName[i], 10) == 0)
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
