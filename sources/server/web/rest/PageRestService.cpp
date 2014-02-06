#include "stdafx.h"
#include "PageRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"


CPageRestService::CPageRestService(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("page")
{
}


CPageRestService::~CPageRestService()
{
}

const std::string & CPageRestService::getRestKeyword()
{
   return m_restKeyword;
}

void CPageRestService::configureDispatcher(CRestDispatcher & dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CPageRestService::getAllPages);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CPageRestService::getOnePage);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("widget"), CPageRestService::getPageWidget);
}

CJson CPageRestService::getOnePage(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];
   
   CPageEntitySerializer hes;
   boost::shared_ptr<CPage> pageFound =  m_dataProvider->getPageRequester()->getPage(boost::lexical_cast<int>(objectId));
   return hes.serialize(*pageFound.get());
}

CJson CPageRestService::getAllPages(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CPageEntitySerializer hes;
   std::vector< boost::shared_ptr<CPage> > hwList = m_dataProvider->getPageRequester()->getPages();
   return CJonCollectionSerializer<CPage>::SerializeCollection(hwList, hes, getRestKeyword());
}

CJson CPageRestService::getPageWidget(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string pageId = "";
   if(parameters.size()>1)
      pageId = parameters[1];

   CWidgetEntitySerializer hes;
   std::vector< boost::shared_ptr<CWidget> > widgetList = m_dataProvider->getWidgetRequester()->getWidgetsForPage(boost::lexical_cast<int>(pageId));
   return CJonCollectionSerializer<CWidget>::SerializeCollection(widgetList, hes, "widget");
}
