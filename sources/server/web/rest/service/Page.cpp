#include "stdafx.h"
#include "Page.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/poco/RestResult.h"
#include "Widget.h"
#include "web/poco/RestDispatcherHelpers.hpp"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CPage::m_restKeyword = std::string("page");

         CPage::CPage(boost::shared_ptr<database::IDataProvider> dataProvider)
            : m_dataProvider(std::move(dataProvider))
         {
         }

         const std::string& CPage::getRestKeyword()
         {
            return m_restKeyword;
         }

         void CPage::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CPage::getAllPages);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CPage::getOnePage);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")(CWidget::getRestKeyword()), CPage::getPageWidget);

            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CPage::addPage, CPage::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword), CPage::updateAllPages, CPage::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CPage::updatePage, CPage::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CPage::deletePage, CPage::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CPage::deleteAllPages, CPage::transactionalMethod);

            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)(CWidget::getRestKeyword()), CPage::addWidgetForPage,
                                                        CPage::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")(CWidget::getRestKeyword()),
                                                        CPage::replaceAllWidgetsForPage, CPage::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*")(CWidget::getRestKeyword()),
                                                        CPage::deleteAllWidgetsForPage, CPage::transactionalMethod);
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CPage::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            // No end point for this service
            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();

            return m_endPoints;
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::transactionalMethod(
            poco::CRestDispatcher::CRestMethodHandler realMethod,
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            auto pTransactionalEngine = m_dataProvider->getTransactionalEngine();
            boost::shared_ptr<shared::serialization::IDataSerializable> result;
            try
            {
               if (pTransactionalEngine)
                  pTransactionalEngine->transactionBegin();
               result = realMethod(parameters, requestContent);
            }
            catch (std::exception& ex)
            {
               result = poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               result = poco::CRestResult::GenerateError("unknown exception widget rest method");
            }

            if (pTransactionalEngine)
            {
               if (poco::CRestResult::isSuccess(*boost::dynamic_pointer_cast<shared::CDataContainer>(result)))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::getOnePage(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const
         {
            if (parameters.size() > 1)
            {
               const auto pageId = boost::lexical_cast<int>(parameters[1]);
               const auto pageFound = m_dataProvider->getPageRequester()->getPage(pageId);
               return poco::CRestResult::GenerateSuccess(pageFound);
            }
            return poco::CRestResult::GenerateError("Invalid parameter count (need page id in url)");
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::getAllPages(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent) const
         {
            const auto pageList = m_dataProvider->getPageRequester()->getPages();
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), pageList);
            return poco::CRestResult::GenerateSuccess(collection);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::getPageWidget(const std::vector<std::string>& parameters,
                                                                                          const std::string& requestContent) const
         {
            if (parameters.size() > 1)
            {
               const auto pageId = parameters[1];

               const auto widgetList = m_dataProvider->getWidgetRequester()->getWidgetsForPage(boost::lexical_cast<int>(pageId));
               shared::CDataContainer collection;
               collection.set(CWidget::getRestKeyword(), widgetList);

               return poco::CRestResult::GenerateSuccess(collection);
            }
            return poco::CRestResult::GenerateError("Invalid parameter count (need page id in url)");
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::addPage(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent) const
         {
            try
            {
               database::entities::CPage pageToAdd;
               pageToAdd.fillFromSerializedString(requestContent);
               const auto idCreated = m_dataProvider->getPageRequester()->addPage(pageToAdd.Name(), pageToAdd.PageOrder());
               const auto pageFound = m_dataProvider->getPageRequester()->getPage(idCreated);
               return poco::CRestResult::GenerateSuccess(pageFound);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new page");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::updatePage(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto pageId = boost::lexical_cast<int>(parameters[1].c_str());

                  database::entities::CPage pageToReplace;
                  pageToReplace.fillFromSerializedString(requestContent);

                  if (pageToReplace.Id() > 0 && pageId == pageToReplace.Id())
                  {
                     m_dataProvider->getPageRequester()->updatePage(pageToReplace.Id(), pageToReplace.Name(), pageToReplace.PageOrder());
                     return poco::CRestResult::GenerateSuccess(m_dataProvider->getPageRequester()->getPage(pageToReplace.Id()));
                  }
                  return poco::CRestResult::GenerateError("The page to replace must have a valid id");
               }
               return poco::CRestResult::GenerateError("Invalid parameter count (need page id in url)");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in replacing a page");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::updateAllPages(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               m_dataProvider->getPageRequester()->removeAllPages();

               auto pagesToUpdate = shared::CDataContainer(requestContent).get<std::vector<boost::shared_ptr<database::entities::CPage>>>("");
               for (const auto& pageToUpdate : pagesToUpdate)
                  m_dataProvider->getPageRequester()->addPage(*pageToUpdate);

               const auto allPages = m_dataProvider->getPageRequester()->getPages();
               shared::CDataContainer collection;
               collection.set(getRestKeyword(), allPages);
               return poco::CRestResult::GenerateSuccess(collection);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in updating all pages");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::deletePage(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto pageId = boost::lexical_cast<int>(parameters[1].c_str());

                  //delete all widgets
                  m_dataProvider->getWidgetRequester()->removeWidgetsInPage(pageId);

                  //remove page
                  m_dataProvider->getPageRequester()->removePage(pageId);
                  return poco::CRestResult::GenerateSuccess();
               }
               return poco::CRestResult::GenerateError("The page to delete must have an id");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in deleting a page");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::deleteAllPages(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const
         {
            try
            {
               //delete all widgets
               m_dataProvider->getWidgetRequester()->removeAllWidgets();

               //remove page
               m_dataProvider->getPageRequester()->removeAllPages();
               return poco::CRestResult::GenerateSuccess();
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in deleting all pages");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::addWidgetForPage(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const
         {
            try
            {
               database::entities::CWidget widgetToAdd;
               widgetToAdd.fillFromSerializedString(requestContent);
               const auto idCreated = m_dataProvider->getWidgetRequester()->addWidget(widgetToAdd);
               const auto widgetFound = m_dataProvider->getWidgetRequester()->getWidget(idCreated);
               return poco::CRestResult::GenerateSuccess(widgetFound);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new widget");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::replaceAllWidgetsForPage(const std::vector<std::string>& parameters,
                                                                                                     const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto pageId = boost::lexical_cast<int>(parameters[1].c_str());

                  //remove all widgets in page
                  m_dataProvider->getWidgetRequester()->removeWidgetsInPage(pageId);

                  //rreate all
                  auto widgetsToAdd = shared::CDataContainer(requestContent).get<std::vector<boost::shared_ptr<database::entities::CWidget>>>("");

                  for (const auto& widgetToAdd : widgetsToAdd)
                     m_dataProvider->getWidgetRequester()->addWidget(*widgetToAdd);

                  return poco::CRestResult::GenerateSuccess();
               }
               return poco::CRestResult::GenerateError("Invalid parameter count (need page id in url)");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new widget");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CPage::deleteAllWidgetsForPage(const std::vector<std::string>& parameters,
                                                                                                    const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto pageId = boost::lexical_cast<int>(parameters[1].c_str());

                  m_dataProvider->getWidgetRequester()->removeWidgetsInPage(pageId);
                  return poco::CRestResult::GenerateSuccess();
               }
               return poco::CRestResult::GenerateError("Invalid parameter count (need page id in url)");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new widget");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
