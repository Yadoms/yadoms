#include "stdafx.h"
#include "WidgetRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "RestDispatcher.h"

CWidgetRestService::CWidgetRestService(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider), m_restKeyword("widget")
{

}


CWidgetRestService::~CWidgetRestService()
{
}

 
void CWidgetRestService::configureDispatcher(CRestDispatcher & dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CWidgetRestService::getAllWidgets);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CWidgetRestService::getOneWidget);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("acquisition"), CWidgetRestService::getWidgetAcquisitions);
}

const std::string & CWidgetRestService::getRestKeyword()
{
   return m_restKeyword;
}


CJson CWidgetRestService::getOneWidget(const std::vector<std::string> & parameters)
{
   CWidgetEntitySerializer hes;

   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];

   boost::shared_ptr<CWidget> widgetFound =  m_dataProvider->getWidgetRequester()->getWidget(boost::lexical_cast<int>(objectId));
   return hes.serialize(*widgetFound.get());
}

CJson CWidgetRestService::getAllWidgets(const std::vector<std::string> & parameters)
{
   CWidgetEntitySerializer hes;
   std::vector< boost::shared_ptr<CWidget> > hwList = m_dataProvider->getWidgetRequester()->getWidgets();
   return CJonCollectionSerializer<CWidget>::SerializeCollection(hwList, hes, getRestKeyword());
}

CJson CWidgetRestService::getWidgetAcquisitions(const std::vector<std::string> & parameters)
{
   CWidgetEntitySerializer hes;
   std::vector< boost::shared_ptr<CWidget> > hwList = m_dataProvider->getWidgetRequester()->getWidgets();
   return CJonCollectionSerializer<CWidget>::SerializeCollection(hwList, hes, getRestKeyword());
}
