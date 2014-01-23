#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

class CWidgetRestService : public IReadRestService
{
public:
   CWidgetRestService(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CWidgetRestService();

public:
   // IReadRestService implementation
   const std::string & getRestKeyword();
   CJson readObject(const std::string & objectId);   
   CJson readObjects();  
   // [END] IReadRestService implementation
   
private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};