#include "stdafx.h"
#include "WidgetRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "RestDispatcher.h"
#include "json/JsonResult.h"

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
   REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword), CWidgetRestService::addWidget);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("*"), CWidgetRestService::deleteOneWidget);
}

const std::string & CWidgetRestService::getRestKeyword()
{
   return m_restKeyword;
}


CJson CWidgetRestService::getOneWidget(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CWidgetEntitySerializer hes;

   std::string objectId = "";
   if(parameters.size()>1)
      objectId = parameters[1];

   boost::shared_ptr<CWidget> widgetFound =  m_dataProvider->getWidgetRequester()->getWidget(boost::lexical_cast<int>(objectId));
   return hes.serialize(*widgetFound.get());
}

CJson CWidgetRestService::getAllWidgets(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CWidgetEntitySerializer hes;
   std::vector< boost::shared_ptr<CWidget> > hwList = m_dataProvider->getWidgetRequester()->getWidgets();
   return CJonCollectionSerializer<CWidget>::SerializeCollection(hwList, hes, getRestKeyword());
}

CJson CWidgetRestService::getWidgetAcquisitions(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CWidgetEntitySerializer hes;
   std::vector< boost::shared_ptr<CWidget> > hwList = m_dataProvider->getWidgetRequester()->getWidgets();
   return CJonCollectionSerializer<CWidget>::SerializeCollection(hwList, hes, getRestKeyword());
}

CJson CWidgetRestService::addWidget(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      CWidgetEntitySerializer hes;
      CWidget widgetToAdd = hes.deserialize(requestContent);
      int idCreated = m_dataProvider->getWidgetRequester()->addWidget(widgetToAdd);
      boost::shared_ptr<CWidget> widgetFound =  m_dataProvider->getWidgetRequester()->getWidget(idCreated);
      return CJsonResult::GenerateSuccess(hes.serialize(*widgetFound.get()));
   }
   catch(std::exception &ex)
   {
      return CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      return CJsonResult::GenerateError("unknown exception in creating a new widget");
   }
}

CJson CWidgetRestService::deleteOneWidget(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   int widgetId = 0;
   if(parameters.size()>1)
   {
      widgetId = boost::lexical_cast<int>(parameters[1]);
      m_dataProvider->getWidgetRequester()->removeWidget(widgetId);
      return CJsonResult::GenerateSuccess();
   }
   return CJsonResult::GenerateError("invalid parameter. Can not retreive widget id in utrl");
}
