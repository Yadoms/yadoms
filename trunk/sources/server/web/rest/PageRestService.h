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
   CJson getOnePage(const std::vector<std::string> & parameters, const CJson & requestContent);   
   CJson getAllPages(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson getPageWidget(const std::vector<std::string> & parameters, const CJson & requestContent);   
   
private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};