#include "stdafx.h"
#include "Widget.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/Result.h"
#include <fstream>

namespace web {
   namespace rest {
      namespace service {

         std::string CWidget::m_restKeyword = std::string("widget");

         CWidget::CWidget(boost::shared_ptr<database::IDataProvider> dataProvider, const std::string & webServerPath)
            :m_dataProvider(dataProvider), m_webServerPath(webServerPath)
         {

         }


         CWidget::~CWidget()
         {
         }


         void CWidget::configureDispatcher(CRestDispatcher & dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CWidget::getAllWidgets);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CWidget::getOneWidget);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("package"), CWidget::findWidgetPackages);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CWidget::addWidget, CWidget::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword), CWidget::replaceAllWidgets, CWidget::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CWidget::updateOneWidget, CWidget::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CWidget::deleteAllWidgets, CWidget::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CWidget::deleteOneWidget, CWidget::transactionalMethod);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const std::string & requestContent)
         {
            boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
            boost::shared_ptr<shared::serialization::IDataSerializable> result;
            try
            {
               if (pTransactionalEngine)
                  pTransactionalEngine->transactionBegin();
               result = realMethod(parameters, requestContent);
            }
            catch (std::exception &ex)
            {
               result = CResult::GenerateError(ex);
            }
            catch (...)
            {
               result = CResult::GenerateError("unknown exception widget rest method");
            }

            if (pTransactionalEngine)
            {
               if (CResult::isSuccess(*boost::dynamic_pointer_cast<shared::CDataContainer>(result)))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }


         const std::string & CWidget::getRestKeyword()
         {
            return m_restKeyword;
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::getOneWidget(const std::vector<std::string> & parameters, const std::string & requestContent)
         {
            try
            {
               if (parameters.size() > 1)
               {
                  int objectId = boost::lexical_cast<int>(parameters[1]);
                  boost::shared_ptr<database::entities::CWidget> widgetFound = m_dataProvider->getWidgetRequester()->getWidget(objectId);
                  return CResult::GenerateSuccess(widgetFound);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve widget id in url");
            }
            catch (std::exception &ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving one widget");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::getAllWidgets(const std::vector<std::string> & parameters, const std::string & requestContent)
         {
            std::vector< boost::shared_ptr<database::entities::CWidget> > widgetList = m_dataProvider->getWidgetRequester()->getWidgets();
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), widgetList);
            return CResult::GenerateSuccess(collection);
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::addWidget(const std::vector<std::string> & parameters, const std::string & requestContent)
         {
            try
            {
               database::entities::CWidget widgetToAdd;
               widgetToAdd.fillFromSerializedString(requestContent);
               int idCreated = m_dataProvider->getWidgetRequester()->addWidget(widgetToAdd);
               boost::shared_ptr<database::entities::CWidget> widgetFound = m_dataProvider->getWidgetRequester()->getWidget(idCreated);
               return CResult::GenerateSuccess(widgetFound);
            }
            catch (std::exception &ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in creating a new widget");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::updateOneWidget(const std::vector<std::string> & parameters, const std::string & requestContent)
         {
            try
            {
               database::entities::CWidget widgetToUpdate;
               widgetToUpdate.fillFromSerializedString(requestContent);


               if (parameters.size() > 1)
               {
                  int objectId = boost::lexical_cast<int>(parameters[1]);

                  if (widgetToUpdate.Id() == objectId)
                  {
                     m_dataProvider->getWidgetRequester()->updateWidget(widgetToUpdate, true);

                     boost::shared_ptr<database::entities::CWidget> wi = m_dataProvider->getWidgetRequester()->getWidget(widgetToUpdate.Id());
                     return CResult::GenerateSuccess(wi);
                  }
                  return CResult::GenerateError("The wiget from URL is different than request content one");
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve widget id in url");
            }
            catch (std::exception &ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating a widget value");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::deleteOneWidget(const std::vector<std::string> & parameters, const std::string & requestContent)
         {
            try
            {
               if (parameters.size() > 1)
               {
                  int widgetId = boost::lexical_cast<int>(parameters[1]);
                  m_dataProvider->getWidgetRequester()->removeWidget(widgetId);
                  return CResult::GenerateSuccess();
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve widget id in url");
            }
            catch (std::exception &ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in deleting one widget");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::replaceAllWidgets(const std::vector<std::string> & parameters, const std::string & requestContent)
         {
            try
            {
               m_dataProvider->getWidgetRequester()->removeAllWidgets();

               std::vector<boost::shared_ptr<database::entities::CWidget> > widgetsToAdd = shared::CDataContainer(requestContent).get<std::vector<boost::shared_ptr<database::entities::CWidget> > >(getRestKeyword());

               for (std::vector<boost::shared_ptr<database::entities::CWidget> >::iterator i = widgetsToAdd.begin(); i != widgetsToAdd.end(); ++i)
               {
                  m_dataProvider->getWidgetRequester()->addWidget(*i->get());
               }

               return CResult::GenerateSuccess();
            }
            catch (std::exception &ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating all widgets");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::deleteAllWidgets(const std::vector<std::string> & parameters, const std::string & requestContent)
         {
            try
            {
               m_dataProvider->getWidgetRequester()->removeAllWidgets();
               return CResult::GenerateSuccess();
            }
            catch (std::exception &ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in deleting all widgets");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::findWidgetPackages(const std::vector<std::string> & parameters, const std::string & requestContent)
         {
            try
            {
               shared::CDataContainer result;

               //construct widget path
               std::string widgetPath = m_webServerPath;
               if (!boost::algorithm::ends_with(widgetPath, "/"))
                  widgetPath += "/";
               widgetPath += "widgets";

               boost::filesystem::path someDir(widgetPath);
               boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end

               if (boost::filesystem::exists(someDir) && boost::filesystem::is_directory(someDir))
               {
                  std::vector<shared::CDataContainer> allData;
                  for (boost::filesystem::directory_iterator dir_iter(someDir); dir_iter != end_itr; ++dir_iter)
                  {
                     if (boost::filesystem::is_directory(*dir_iter))
                     {
                        //dir_iter->m_path
                        std::string packageFile = dir_iter->path().string();
                        if (!boost::algorithm::ends_with(packageFile, "/"))
                           packageFile += "/";
                        packageFile += "package.json";

                        boost::filesystem::path packageFileP(packageFile);

                        if (boost::filesystem::exists(packageFileP) && boost::filesystem::is_regular_file(packageFileP))
                        {
                           std::ifstream ifs(packageFile.c_str(), std::ios::in);
                           std::stringstream ss;
                           ss << ifs.rdbuf();
                           allData.push_back(shared::CDataContainer(ss.str()));
                        }
                     }
                  }
                  result.set< std::vector<shared::CDataContainer> >("package", allData);
                  return CResult::GenerateSuccess(result);
               }
               return CResult::GenerateError(widgetPath + " is not a valid directory.");
            }
            catch (std::exception &ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in finding wWidget packages");
            }
         }


      } //namespace service
   } //namespace rest
} //namespace web 
