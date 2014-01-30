#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"


class CWidgetRestService : public IRestService
{
public:
   CWidgetRestService(boost::shared_ptr<IDataProvider> dataProvider);
   virtual ~CWidgetRestService();

public:
   // IRestService implementation
   virtual void configureDispatcher(CRestDispatcher & dispatcher);
   // [END] IRestService implementation
   
   
   const std::string & getRestKeyword();

public:
   CJson getOneWidget(const std::vector<std::string> & parameters);
   CJson getAllWidgets(const std::vector<std::string> & parameters);
   CJson getWidgetAcquisitions(const std::vector<std::string> & parameters);

private:
   boost::shared_ptr<IDataProvider> m_dataProvider;
   std::string m_restKeyword;
};