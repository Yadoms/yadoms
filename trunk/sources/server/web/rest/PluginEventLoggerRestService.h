#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

class CPluginEventLoggerRestService : public IRestService
{
public:
   CPluginEventLoggerRestService(boost::shared_ptr<database::IDataProvider> dataProvider);
   virtual ~CPluginEventLoggerRestService();

public:
    // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation

   const std::string & getRestKeyword();
   
   CJson getLogsForPluginName(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson getLogsForPluginNameFromDate(const std::vector<std::string> & parameters, const CJson & requestContent);  

private:
   boost::shared_ptr<database::IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};