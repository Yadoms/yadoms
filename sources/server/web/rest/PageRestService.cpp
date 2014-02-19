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

   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPageRestService::addPage, CPageRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPageRestService::updatePage, CPageRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPageRestService::deletePage, CPageRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPageRestService::deleteAllPages, CPageRestService::transactionalMethod);

   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")(CWidgetRestService::getRestKeyword()), CPageRestService::addWidgetForPage, CPageRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")(CWidgetRestService::getRestKeyword()), CPageRestService::replaceAllWidgetsForPage, CPageRestService::transactionalMethod);
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*")(CWidgetRestService::getRestKeyword()), CPageRestService::deleteAllWidgetsForPage, CPageRestService::transactionalMethod);
}


CJson CPageRestService::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const CJson & requestContent)
{
   boost::shared_ptr<ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
   CJson result;
   try
   {
      if(pTransactionalEngine)
         pTransactionalEngine->transactionBegin();
      result = realMethod(parameters, requestContent);
   }
   catch(std::exception &ex)
   {
      result = CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      result = CJsonResult::GenerateError("unknown exception widget rest method");
   }

   if(pTransactionalEngine)
   {
      if(CJsonResult::isSuccess(result))
         pTransactionalEngine->transactionCommit();
      else
         pTransactionalEngine->transactionRollback();
   }
   return result;
}



CJson CPageRestService::getOnePage(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   int pageId = 0;
   if(parameters.size()>1)
   {
      pageId = boost::lexical_cast<int>(parameters[1]);
      CPageEntitySerializer hes;
      boost::shared_ptr<CPage> pageFound =  m_dataProvider->getPageRequester()->getPage(pageId);
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

CJson CPageRestService::addPage(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      CPageEntitySerializer hes;
      boost::shared_ptr<CPage> pageToAdd = hes.deserialize(requestContent);
      int idCreated = m_dataProvider->getPageRequester()->addPage(pageToAdd->getName(), pageToAdd->getPageOrder());
      boost::shared_ptr<CPage> pageFound =  m_dataProvider->getPageRequester()->getPage(idCreated);
      return CJsonResult::GenerateSuccess(hes.serialize(*pageFound.get()));
   }
   catch(std::exception &ex)
   {
      return CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      return CJsonResult::GenerateError("unknown exception in creating a new page");
   }
}

CJson CPageRestService::updatePage(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      int pageId = 0;
      if(parameters.size()>1)
      {
         pageId = boost::lexical_cast<int>(parameters[1].c_str());

         CPageEntitySerializer hes;
         boost::shared_ptr<CPage> pageToReplace = hes.deserialize(requestContent);
         if(pageToReplace->getId() > 0 && pageId == pageToReplace->getId())
         {
            m_dataProvider->getPageRequester()->updatePage(pageToReplace->getId(), pageToReplace->getName(), pageToReplace->getPageOrder());
            return CJsonResult::GenerateSuccess(hes.serialize(*m_dataProvider->getPageRequester()->getPage(pageToReplace->getId())));
         }
         else
         {
            return CJsonResult::GenerateError("The page to replace must have a valid id");
         }
      }
   }
   catch(std::exception &ex)
   {
      return CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      return CJsonResult::GenerateError("unknown exception in replacing a page");
   }
}

CJson CPageRestService::deletePage(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      int pageId = 0;
      if(parameters.size()>1)
      {
         pageId = boost::lexical_cast<int>(parameters[1].c_str());

         //delete all widgets
         m_dataProvider->getWidgetRequester()->removeWidgetsInPage(pageId);

         //remove page
         m_dataProvider->getPageRequester()->removePage(pageId);
         return CJsonResult::GenerateSuccess();
      }
      else
      {
         return CJsonResult::GenerateError("The page to delete must have an id");
      }
   }
   catch(std::exception &ex)
   {
      return CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      return CJsonResult::GenerateError("unknown exception in deleting a page");
   }
}

CJson CPageRestService::deleteAllPages(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   try
   {
      //delete all widgets
      m_dataProvider->getWidgetRequester()->removeAllWidgets();

      //remove page
      m_dataProvider->getPageRequester()->removeAllPages();
      return CJsonResult::GenerateSuccess();
   }
   catch(std::exception &ex)
   {
      return CJsonResult::GenerateError(ex);
   }
   catch(...)
   {
      return CJsonResult::GenerateError("unknown exception in deleting all pages");
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
