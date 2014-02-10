#include "stdafx.h"
#include "PageRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "json/JsonResult.h"
#include "WidgetRestService.h"

std::string CPageRestService::m_restKeyword= std::string("page");

CPageRestService::CPageRestService(boost::shared_ptr<IDataProvider> dataProvider)
   :m_dataProvider(dataProvider)
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
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")(CWidgetRestService::getRestKeyword()), CPageRestService::getPageWidget);

   REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("*")(CWidgetRestService::getRestKeyword()), CPageRestService::addWidgetForPage);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword)("*")(CWidgetRestService::getRestKeyword()), CPageRestService::replaceAllWidgetsForPage);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword)("*")(CWidgetRestService::getRestKeyword()), CPageRestService::deleteAllWidgetsForPage);


}

CJson CPageRestService::getOnePage(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string objectId = "";
   if(parameters.size()>1)
   {
      objectId = parameters[1];

      CPageEntitySerializer hes;
      boost::shared_ptr<CPage> pageFound =  m_dataProvider->getPageRequester()->getPage(boost::lexical_cast<int>(objectId));
      return CJsonResult::GenerateSuccess(hes.serialize(*pageFound.get()));
   }
   else
   {
      return CJsonResult::GenerateError("Invalid parameter count (need page id in url)");
   }
}

CJson CPageRestService::getAllPages(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CPageEntitySerializer hes;
   std::vector< boost::shared_ptr<CPage> > hwList = m_dataProvider->getPageRequester()->getPages();
   return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<CPage>::SerializeCollection(hwList, hes, getRestKeyword()));
}

CJson CPageRestService::getPageWidget(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   std::string pageId = "";
   if(parameters.size()>1)
   {
      pageId = parameters[1];

      CWidgetEntitySerializer hes;
      std::vector< boost::shared_ptr<CWidget> > widgetList = m_dataProvider->getWidgetRequester()->getWidgetsForPage(boost::lexical_cast<int>(pageId));
      return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<CWidget>::SerializeCollection(widgetList, hes, CWidgetRestService::getRestKeyword()));
   }
   else
   {
      return CJsonResult::GenerateError("Invalid parameter count (need page id in url)");
   }
}


CJson CPageRestService::addWidgetForPage(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      CWidgetEntitySerializer hes;
      boost::shared_ptr<CWidget> widgetToAdd = hes.deserialize(requestContent);
      int idCreated = m_dataProvider->getWidgetRequester()->addWidget(*widgetToAdd);
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

CJson CPageRestService::replaceAllWidgetsForPage(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      int pageId = 0;
      if(parameters.size()>1)
      {
         pageId = boost::lexical_cast<int>(parameters[1].c_str());

         //remove all widgets in page
         m_dataProvider->getWidgetRequester()->removeWidgetsInPage(pageId);

         //crreate all
         CWidgetEntitySerializer hes;
         std::vector<boost::shared_ptr<CWidget> > widgetsToAdd = CJsonCollectionSerializer<CWidget>::DeserializeCollection(requestContent, hes, getRestKeyword());
         BOOST_FOREACH(boost::shared_ptr<CWidget> pw, widgetsToAdd)
         {
            m_dataProvider->getWidgetRequester()->addWidget(*pw);
         }
         return CJsonResult::GenerateSuccess();
      }
      else
      {
         return CJsonResult::GenerateError("Invalid parameter count (need page id in url)");
      }
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

CJson CPageRestService::deleteAllWidgetsForPage(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      int pageId = 0;
      if(parameters.size()>1)
      {
         pageId = boost::lexical_cast<int>(parameters[1].c_str());

         m_dataProvider->getWidgetRequester()->removeWidgetsInPage(pageId);
         return CJsonResult::GenerateSuccess();
      }
      else
      {
         return CJsonResult::GenerateError("Invalid parameter count (need page id in url)");
      }
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
