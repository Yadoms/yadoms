#include "stdafx.h"
#include "Page.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include "Widget.h"

namespace web { namespace rest { namespace service {

   std::string CPage::m_restKeyword= std::string("page");

   CPage::CPage(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider)
   {
   }


   CPage::~CPage()
   {
   }

   const std::string & CPage::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CPage::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CPage::getAllPages);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CPage::getOnePage);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")(CWidget::getRestKeyword()), CPage::getPageWidget);

      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPage::addPage, CPage::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword), CPage::updateAllPages, CPage::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPage::updatePage, CPage::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPage::deletePage, CPage::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPage::deleteAllPages, CPage::transactionalMethod);

      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")(CWidget::getRestKeyword()), CPage::addWidgetForPage, CPage::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")(CWidget::getRestKeyword()), CPage::replaceAllWidgetsForPage, CPage::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*")(CWidget::getRestKeyword()), CPage::deleteAllWidgetsForPage, CPage::transactionalMethod);
   }


   shared::CDataContainer CPage::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
      shared::CDataContainer result;
      try
      {
         if(pTransactionalEngine)
            pTransactionalEngine->transactionBegin();
         result = realMethod(parameters, requestContent);
      }
      catch(std::exception &ex)
      {
         result = web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         result = web::rest::CResult::GenerateError("unknown exception widget rest method");
      }

      if(pTransactionalEngine)
      {
         if(web::rest::CResult::isSuccess(result))
            pTransactionalEngine->transactionCommit();
         else
            pTransactionalEngine->transactionRollback();
      }
      return result;
   }



   shared::CDataContainer CPage::getOnePage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if(parameters.size()>1)
      {
         int pageId = boost::lexical_cast<int>(parameters[1]);
         boost::shared_ptr<database::entities::CPage> pageFound =  m_dataProvider->getPageRequester()->getPage(pageId);
         return web::rest::CResult::GenerateSuccess(pageFound);
      }
      else
      {
         return web::rest::CResult::GenerateError("Invalid parameter count (need page id in url)");
      }
   }

   shared::CDataContainer CPage::getAllPages(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CPage> > pageList = m_dataProvider->getPageRequester()->getPages();
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), pageList);
      return web::rest::CResult::GenerateSuccess(collection);
   }

   shared::CDataContainer CPage::getPageWidget(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string pageId = "";
      if(parameters.size()>1)
      {
         pageId = parameters[1];

         std::vector< boost::shared_ptr<database::entities::CWidget> > widgetList = m_dataProvider->getWidgetRequester()->getWidgetsForPage(boost::lexical_cast<int>(pageId));
         shared::CDataContainer collection;
         collection.set(getRestKeyword(), widgetList);

         //std::vector< boost::shared_ptr<database::entities::CWidget> > widgetList2 = collection.get< std::vector< boost::shared_ptr<database::entities::CWidget> > >(getRestKeyword());
         return web::rest::CResult::GenerateSuccess(collection);
      }
      else
      {
         return web::rest::CResult::GenerateError("Invalid parameter count (need page id in url)");
      }
   }

   shared::CDataContainer CPage::addPage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         database::entities::CPage pageToAdd;
         pageToAdd.fillFromContent(requestContent);
         int idCreated = m_dataProvider->getPageRequester()->addPage(pageToAdd.Name(), pageToAdd.PageOrder());
         boost::shared_ptr<database::entities::CPage> pageFound =  m_dataProvider->getPageRequester()->getPage(idCreated);
         return web::rest::CResult::GenerateSuccess(pageFound);
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in creating a new page");
      }
   }

   shared::CDataContainer CPage::updatePage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int pageId = boost::lexical_cast<int>(parameters[1].c_str());

            database::entities::CPage pageToReplace;
            pageToReplace.fillFromContent(requestContent);

            if(pageToReplace.Id() > 0 && pageId == pageToReplace.Id())
            {
               m_dataProvider->getPageRequester()->updatePage(pageToReplace.Id(), pageToReplace.Name(), pageToReplace.PageOrder());
               return web::rest::CResult::GenerateSuccess(m_dataProvider->getPageRequester()->getPage(pageToReplace.Id()));
            }
            else
            {
               return web::rest::CResult::GenerateError("The page to replace must have a valid id");
            }
         }
         else
         {
            return web::rest::CResult::GenerateError("Invalid parameter count (need page id in url)");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in replacing a page");
      }
   }




   shared::CDataContainer CPage::updateAllPages(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         m_dataProvider->getPageRequester()->removeAllPages();

         std::vector<boost::shared_ptr<database::entities::CPage> > pagesToUpdate = requestContent.get< std::vector<boost::shared_ptr<database::entities::CPage> > >(getRestKeyword());
         BOOST_FOREACH(boost::shared_ptr<database::entities::CPage> page, pagesToUpdate)
         {
            m_dataProvider->getPageRequester()->addPage(*page);
         }

         std::vector<boost::shared_ptr<database::entities::CPage> > allPages = m_dataProvider->getPageRequester()->getPages();
         shared::CDataContainer collection;
         collection.set(getRestKeyword(), allPages);
         return web::rest::CResult::GenerateSuccess(collection);

      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in updating all pages");
      }
   }





   shared::CDataContainer CPage::deletePage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int pageId = boost::lexical_cast<int>(parameters[1].c_str());

            //delete all widgets
            m_dataProvider->getWidgetRequester()->removeWidgetsInPage(pageId);

            //remove page
            m_dataProvider->getPageRequester()->removePage(pageId);
            return web::rest::CResult::GenerateSuccess();
         }
         else
         {
            return web::rest::CResult::GenerateError("The page to delete must have an id");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in deleting a page");
      }
   }

   shared::CDataContainer CPage::deleteAllPages(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         //delete all widgets
         m_dataProvider->getWidgetRequester()->removeAllWidgets();

         //remove page
         m_dataProvider->getPageRequester()->removeAllPages();
         return web::rest::CResult::GenerateSuccess();
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in deleting all pages");
      }
   }

   shared::CDataContainer CPage::addWidgetForPage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         database::entities::CWidget widgetToAdd;
         widgetToAdd.fillFromContent(requestContent);
         int idCreated = m_dataProvider->getWidgetRequester()->addWidget(widgetToAdd);
         boost::shared_ptr<database::entities::CWidget> widgetFound =  m_dataProvider->getWidgetRequester()->getWidget(idCreated);
         return web::rest::CResult::GenerateSuccess(widgetFound);
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in creating a new widget");
      }
   }

   shared::CDataContainer CPage::replaceAllWidgetsForPage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int pageId = boost::lexical_cast<int>(parameters[1].c_str());

            //remove all widgets in page
            m_dataProvider->getWidgetRequester()->removeWidgetsInPage(pageId);

            //crreate all
            std::vector<boost::shared_ptr<database::entities::CWidget> > widgetsToAdd = requestContent.get< std::vector<boost::shared_ptr<database::entities::CWidget> > >(getRestKeyword());
            BOOST_FOREACH(boost::shared_ptr<database::entities::CWidget> pw, widgetsToAdd)
            {
               m_dataProvider->getWidgetRequester()->addWidget(*pw);
            }
            return web::rest::CResult::GenerateSuccess();
         }
         else
         {
            return web::rest::CResult::GenerateError("Invalid parameter count (need page id in url)");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in creating a new widget");
      }
   }

   shared::CDataContainer CPage::deleteAllWidgetsForPage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int pageId = boost::lexical_cast<int>(parameters[1].c_str());

            m_dataProvider->getWidgetRequester()->removeWidgetsInPage(pageId);
            return web::rest::CResult::GenerateSuccess();
         }
         else
         {
            return web::rest::CResult::GenerateError("Invalid parameter count (need page id in url)");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in creating a new widget");
      }

   }


} //namespace service
} //namespace rest
} //namespace web 
