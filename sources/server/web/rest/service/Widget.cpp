#include "stdafx.h"
#include "Widget.h"
#include "web/poco/RestResult.h"
#include <fstream>

#include "RestEndPoint.h"
#include "shared/exception/EmptyResult.hpp"
#include "web/poco/RestDispatcherHelpers.hpp"
#include "web/rest/CreatedAnswer.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/Helpers.h"
#include "web/rest/NoContentAnswer.h"

using namespace web::rest::service;

std::string CWidget::m_restKeyword = std::string("widget");

CWidget::CWidget(const boost::shared_ptr<database::IDataProvider>& dataProvider,
                 std::string webServerPath)
   : m_webServerPathV1(std::move(webServerPath)),
     m_dataProvider(dataProvider)
{
}

void CWidget::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CWidget::getAllWidgetsV1)
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CWidget::getOneWidgetV1)
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("package"), CWidget::findWidgetPackagesV1)
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CWidget::addWidgetV1, CWidget::transactionalMethod)
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CWidget::updateOneWidgetV1,
                                               CWidget::transactionalMethod)
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CWidget::deleteAllWidgetsV1,
                                               CWidget::transactionalMethod)
   REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CWidget::deleteOneWidgetV1,
                                               CWidget::transactionalMethod)
}

boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CWidget::endPoints()
{
   if (m_endPoints != nullptr)
      return m_endPoints;

   m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();

   m_endPoints->push_back(MAKE_ENDPOINT(kGet, "widgets", getWidgetsV2));
   m_endPoints->push_back(MAKE_ENDPOINT(kGet, "widgets/{id}", getWidgetsV2));

   m_endPoints->push_back(MAKE_ENDPOINT(kPost, "widgets", createWidgetV2));

   return m_endPoints;
}

boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::transactionalMethod(
   const poco::CRestDispatcher::CRestMethodHandler& realMethod,
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

boost::shared_ptr<web::rest::IAnswer> CWidget::getWidgetsV2(const boost::shared_ptr<IRequest>& request) const
{
   try
   {
      // ID
      const auto widgetId = request->pathVariableExists("id")
                               ? std::make_optional(static_cast<int>(std::stol(request->pathVariable("id"))))
                               : std::optional<int>();

      // Filtering
      const auto page = request->queryParamExists("page")
                           ? std::make_optional(static_cast<int>(std::stol(request->queryParam("page"))))
                           : std::optional<int>();

      // Process the request
      std::vector<boost::shared_ptr<database::entities::CWidget>> widgets;
      if (widgetId)
         widgets.push_back(m_dataProvider->getWidgetRequester()->getWidget(*widgetId));
      else if (page)
         widgets = m_dataProvider->getWidgetRequester()->getWidgetsForPage(*page);
      else
         widgets = m_dataProvider->getWidgetRequester()->getWidgets();

      if (widgets.empty())
         return boost::make_shared<CNoContentAnswer>();

      // Get requested props
      const auto props = request->queryParamAsList("prop");
      std::vector<boost::shared_ptr<shared::CDataContainer>> widgetsEntries;
      for (const auto& widget : widgets)
      {
         auto widgetEntry = boost::make_shared<shared::CDataContainer>();
         if (props->empty() || props->find("id") != props->end())
            widgetEntry->set("id", widget->Id());
         if (props->empty() || props->find("page") != props->end())
            widgetEntry->set("page", widget->IdPage());
         if (props->empty() || props->find("type") != props->end())
            widgetEntry->set("type", widget->Type());
         if (props->empty() || props->find("position") != props->end())
            widgetEntry->set("position", widget->Position());
         if (props->empty() || props->find("title") != props->end())
            widgetEntry->set("title", widget->Title());
         if (props->empty() || props->find("configuration") != props->end())
            widgetEntry->set("configuration", widget->Configuration());

         widgetsEntries.push_back(widgetEntry);
      }

      return CHelpers::formatGetMultiItemsAnswer(widgets.size() == 1,
                                                 widgetsEntries,
                                                 "widgets");
   }
   catch (const shared::exception::CEmptyResult& exception)
   {
      YADOMS_LOG(error) << "Error processing getWidgets request : " << exception.what();
      return boost::make_shared<CNoContentAnswer>();
   }
   catch (const shared::exception::COutOfRange& exception)
   {
      YADOMS_LOG(error) << "Error processing getWidgets request : " << exception.what();
      return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
   }
   catch (const std::exception& exception)
   {
      YADOMS_LOG(error) << "Error processing getWidgets request : " << exception.what();
      return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                              "Fail to get widgets");
   }
}

boost::shared_ptr<web::rest::IAnswer> CWidget::createWidgetV2(const boost::shared_ptr<IRequest>& request) const
{
   try
   {
      return CHelpers::transactionalMethodV2(
         request,
         m_dataProvider,
         [this](const auto& req) -> boost::shared_ptr<IAnswer>
         {
            shared::CDataContainer widgetToCreate(req->body());

            database::entities::CWidget widget;
            widget.fillFromContent(widgetToCreate);
            const auto idCreated = m_dataProvider->getWidgetRequester()->addWidget(widget);
            return boost::make_shared<CCreatedAnswer>("widgets/" + std::to_string(idCreated));
         });
   }
   catch (const std::exception&)
   {
      return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                              "Fail to create plugin instance");
   }
}


const std::string& CWidget::getRestKeyword()
{
   return m_restKeyword;
}


boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::getOneWidgetV1(const std::vector<std::string>& parameters,
                                                                                    const std::string& requestContent) const
{
   try
   {
      if (parameters.size() > 1)
      {
         const auto objectId = boost::lexical_cast<int>(parameters[1]);
         const auto widgetFound = m_dataProvider->getWidgetRequester()->getWidget(objectId);
         return poco::CRestResult::GenerateSuccess(widgetFound);
      }
      return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve widget id in url");
   }
   catch (std::exception& ex)
   {
      return poco::CRestResult::GenerateError(ex);
   }
   catch (...)
   {
      return poco::CRestResult::GenerateError("unknown exception in retrieving one widget");
   }
}

boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::getAllWidgetsV1(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const
{
   const auto widgetList = m_dataProvider->getWidgetRequester()->getWidgets();
   shared::CDataContainer collection;
   collection.set(getRestKeyword(), widgetList);
   return poco::CRestResult::GenerateSuccess(collection);
}


boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::addWidgetV1(const std::vector<std::string>& parameters,
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

boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::updateOneWidgetV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const
{
   try
   {
      database::entities::CWidget widgetToUpdate;
      widgetToUpdate.fillFromSerializedString(requestContent);


      if (parameters.size() > 1)
      {
         const auto objectId = boost::lexical_cast<int>(parameters[1]);

         if (widgetToUpdate.Id() == objectId)
         {
            m_dataProvider->getWidgetRequester()->updateWidget(widgetToUpdate, true);

            const auto wi = m_dataProvider->getWidgetRequester()->getWidget(widgetToUpdate.Id());
            return poco::CRestResult::GenerateSuccess(wi);
         }
         return poco::CRestResult::GenerateError("The widget from URL is different than request content one");
      }
      return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve widget id in url");
   }
   catch (std::exception& ex)
   {
      return poco::CRestResult::GenerateError(ex);
   }
   catch (...)
   {
      return poco::CRestResult::GenerateError("unknown exception in updating a widget value");
   }
}


boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::deleteOneWidgetV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const
{
   try
   {
      if (parameters.size() > 1)
      {
         const auto widgetId = boost::lexical_cast<int>(parameters[1]);
         m_dataProvider->getWidgetRequester()->removeWidget(widgetId);
         return poco::CRestResult::GenerateSuccess();
      }
      return poco::CRestResult::GenerateError("invalid parameter. Can not retrieve widget id in url");
   }
   catch (std::exception& ex)
   {
      return poco::CRestResult::GenerateError(ex);
   }
   catch (...)
   {
      return poco::CRestResult::GenerateError("unknown exception in deleting one widget");
   }
}

boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::replaceAllWidgetsV1(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const
{
   try
   {
      m_dataProvider->getWidgetRequester()->removeAllWidgets();
      const auto widgetsToAdd = shared::CDataContainer(requestContent).get<std::vector<boost::shared_ptr<database::entities::CWidget>>>(
         getRestKeyword());
      for (const auto& i : widgetsToAdd)
         m_dataProvider->getWidgetRequester()->addWidget(*i);
      return poco::CRestResult::GenerateSuccess();
   }
   catch (std::exception& ex)
   {
      return poco::CRestResult::GenerateError(ex);
   }
   catch (...)
   {
      return poco::CRestResult::GenerateError("unknown exception in updating all widgets");
   }
}

boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::deleteAllWidgetsV1(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent) const
{
   try
   {
      m_dataProvider->getWidgetRequester()->removeAllWidgets();
      return poco::CRestResult::GenerateSuccess();
   }
   catch (std::exception& ex)
   {
      return poco::CRestResult::GenerateError(ex);
   }
   catch (...)
   {
      return poco::CRestResult::GenerateError("unknown exception in deleting all widgets");
   }
}


boost::shared_ptr<shared::serialization::IDataSerializable> CWidget::findWidgetPackagesV1(const std::vector<std::string>& parameters,
                                                                                          const std::string& requestContent) const
{
   try
   {
      shared::CDataContainer result;

      //construct widget path
      auto widgetPath = m_webServerPathV1;
      if (!boost::algorithm::ends_with(widgetPath, "/"))
         widgetPath += "/";
      widgetPath += "widgets";

      const boost::filesystem::path someDir(widgetPath);
      const boost::filesystem::directory_iterator endItr; // default construction yields past-the-end

      if (boost::filesystem::exists(someDir) && boost::filesystem::is_directory(someDir))
      {
         result.createArray("package");
         for (boost::filesystem::directory_iterator dirItr(someDir); dirItr != endItr; ++dirItr)
         {
            if (boost::filesystem::is_directory(*dirItr))
            {
               //dirItr->m_path
               auto packageFile = dirItr->path().string();
               if (!boost::algorithm::ends_with(packageFile, "/"))
                  packageFile += "/";
               packageFile += "package.json";

               boost::filesystem::path packageFileP(packageFile);

               if (boost::filesystem::exists(packageFileP) && boost::filesystem::is_regular_file(packageFileP))
               {
                  std::ifstream ifs(packageFile.c_str(), std::ios::in);
                  std::stringstream ss;
                  ss << ifs.rdbuf();
                  result.appendArray("package", shared::CDataContainer(ss.str()));
               }
            }
         }

         return poco::CRestResult::GenerateSuccess(result);
      }
      return poco::CRestResult::GenerateError(widgetPath + " is not a valid directory.");
   }
   catch (std::exception& ex)
   {
      return poco::CRestResult::GenerateError(ex);
   }
   catch (...)
   {
      return poco::CRestResult::GenerateError("unknown exception in finding wWidget packages");
   }
}
