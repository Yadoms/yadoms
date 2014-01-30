#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

class CPageRestService : public IRestService
{
public:
   CPageRestService(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CPageRestService();

public:
    // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation

   const std::string & getRestKeyword();
   CJson getOnePage(const std::vector<std::string> & parameters);   
   CJson getAllPages(const std::vector<std::string> & parameters);  
   CJson getPageWidget(const std::vector<std::string> & parameters);   
   
private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};