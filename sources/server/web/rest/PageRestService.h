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

   static const std::string & getRestKeyword();
   CJson getOnePage(const std::vector<std::string> & parameters, const CJson & requestContent);   
   CJson getAllPages(const std::vector<std::string> & parameters, const CJson & requestContent);  
   CJson getPageWidget(const std::vector<std::string> & parameters, const CJson & requestContent);   
   
   CJson addWidgetForPage(const std::vector<std::string> & parameters, const CJson & requestContent);   
   CJson replaceAllWidgetsForPage(const std::vector<std::string> & parameters, const CJson & requestContent);   
   CJson deleteAllWidgetsForPage(const std::vector<std::string> & parameters, const CJson & requestContent);   

private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   static std::string m_restKeyword;
};