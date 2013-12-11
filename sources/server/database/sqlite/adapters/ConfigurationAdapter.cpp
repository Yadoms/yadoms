#include "stdafx.h"
#include "ConfigurationAdapter.h"
#include "tools/Log.h"

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

      std::string columnSection("section");
      std::string columnName("name");
      std::string columnValue("value");
      std::string columnDefaultValue("default_value");
      std::string columnDescription("description");
      std::string columnLastModificationDate("last_modification_date");

      for(int i=0; i<column ; i++)
      {
         std::string colName(columnNames[i]);

         if(boost::iequals(columnSection, colName))
            newConfiguration->setSection( columValues[i] );
         else if(boost::iequals(columnName, colName))
            newConfiguration->setName( columValues[i] );
         else if(boost::iequals(columnValue, colName))
            newConfiguration->setValue( columValues[i] );
         else if(boost::iequals(columnDefaultValue, colName))
            newConfiguration->setDefaultValue( columValues[i] );
         else if(boost::iequals(columnDescription, colName))
            newConfiguration->setDescription( columValues[i] );
         else if(boost::iequals(columnLastModificationDate, colName))
            newConfiguration->setLastModificationDate( boost::gregorian::from_string(columValues[i]) );
         else
         {
            //ignore it
            YADOMS_LOG(warning) << "Unknown column Name= " << columnName[i] << " Value=" << columValues[i];
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
