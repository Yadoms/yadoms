#include "GenericAdapter.h"
#include "boost/log/trivial.hpp"

CGenericAdapter::CGenericAdapter()
{
}

CGenericAdapter::~CGenericAdapter()
{
}

bool CGenericAdapter::adapt(int column, char** columValues, char** columnName)
{
   bool returnValue = false;
   if(column == 0 || columValues == NULL || columnName == NULL)
   {
      BOOST_LOG_TRIVIAL(warning) << "Invalid arguments";
      returnValue = false;
   }
   else
   {
      std::map<std::string, std::string> newRow;

      for(int i=0; i<column ; i++)
         newRow.insert(std::make_pair(columnName[i], columValues[i]));

      m_results.push_back(newRow);
      returnValue = true;
   }
   return returnValue;
}

std::vector<std::map<std::string, std::string>> CGenericAdapter::getResults()  
{ 
   return m_results; 
}
