#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

class CEventLoggerRestService : public IRestService
{
public:
   CEventLoggerRestService(boost::shared_ptr<server::database::IDataProvider> dataProvider);
   virtual ~CEventLoggerRestService();

public:
    // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation

   const std::string & getRestKeyword();
 

private:
   boost::shared_ptr<server::database::IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};