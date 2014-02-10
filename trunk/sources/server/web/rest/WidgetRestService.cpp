#include "stdafx.h"
#include "WidgetRestService.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "RestDispatcher.h"
#include "json/JsonResult.h"
#include "AcquisitionRestService.h"


std::string CWidgetRestService::m_restKeyword= std::string("widget");

CWidgetRestService::CWidgetRestService(boost::shared_ptr<IDataProvider> dataProvider, const std::string & webServerPath)
   :m_dataProvider(dataProvider), m_webServerPath(webServerPath)
{

}


CWidgetRestService::~CWidgetRestService()
{
}


void CWidgetRestService::configureDispatcher(CRestDispatcher & dispatcher)
{
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CWidgetRestService::getAllWidgets);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CWidgetRestService::getOneWidget);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")(CAcquisitionRestService::getRestKeyword()), CWidgetRestService::getWidgetAcquisitions);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("packages"), CWidgetRestService::findWidgetPackages);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword), CWidgetRestService::addWidget);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "PUT", (m_restKeyword), CWidgetRestService::replaceAllWidgets);
   REGISTER_DISPATCHER_HANDLER(dispatcher, "DELETE", (m_restKeyword), CWidgetRestService::deleteAllWidgets);
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
   return CJsonResult::GenerateSuccess(hes.serialize(*widgetFound.get()));
}

CJson CWidgetRestService::getAllWidgets(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CWidgetEntitySerializer hes;
   std::vector< boost::shared_ptr<CWidget> > hwList = m_dataProvider->getWidgetRequester()->getWidgets();
   return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<CWidget>::SerializeCollection(hwList, hes, getRestKeyword()));
}

CJson CWidgetRestService::getWidgetAcquisitions(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CWidgetEntitySerializer hes;
   std::vector< boost::shared_ptr<CWidget> > hwList = m_dataProvider->getWidgetRequester()->getWidgets();
   return CJsonResult::GenerateSuccess(CJsonCollectionSerializer<CWidget>::SerializeCollection(hwList, hes, CAcquisitionRestService::getRestKeyword()));
}

CJson CWidgetRestService::addWidget(const std::vector<std::string> & parameters, const CJson & requestContent)
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


CJson CWidgetRestService::replaceAllWidgets(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   m_dataProvider->getWidgetRequester()->removeAllWidgets();

   CWidgetEntitySerializer hes;
   std::vector<boost::shared_ptr<CWidget> > widgetsToAdd = CJsonCollectionSerializer<CWidget>::DeserializeCollection(requestContent, hes, getRestKeyword());

   BOOST_FOREACH(boost::shared_ptr<CWidget> pw, widgetsToAdd)
   {
      m_dataProvider->getWidgetRequester()->addWidget(*pw);
   }

   return CJsonResult::GenerateSuccess();
}

CJson CWidgetRestService::deleteAllWidgets(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   m_dataProvider->getWidgetRequester()->removeAllWidgets();
   return CJsonResult::GenerateSuccess();
}


CJson CWidgetRestService::findWidgetPackages(const std::vector<std::string> & parameters, const CJson & requestContent)
{
   CJson result;

   //construct widget path
   std::string widgetPath = m_webServerPath;
   if(!boost::algorithm::ends_with(widgetPath, "/"))
      widgetPath += "/";
   widgetPath += "widgets";

   boost::filesystem::path someDir(widgetPath);
   boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
   CJsonSerializer ser;

   if ( boost::filesystem::exists(someDir) && boost::filesystem::is_directory(someDir))
   {
      CJson allData;
      for( boost::filesystem::directory_iterator dir_iter(someDir) ; dir_iter != end_itr ; ++dir_iter)
      {
         if(boost::filesystem::is_directory(*dir_iter))
         {
            //dir_iter->m_path
            std::string packageFile = dir_iter->path().string();
            if(!boost::algorithm::ends_with(packageFile, "/"))
               packageFile += "/";
            packageFile += "package.json";

            boost::filesystem::path packageFileP(packageFile);

            if ( boost::filesystem::exists(packageFileP) && boost::filesystem::is_regular_file(packageFileP))
            {
               std::ifstream ifs;
               ifs.open(packageFile.c_str(),std::ios::in);

               
               std::stringstream ss;
               ss << ifs.rdbuf();
               CJson jsonFromPackageFile;
               ser.deserialize(ss.str(), jsonFromPackageFile);
               allData.push_back(std::make_pair("", jsonFromPackageFile));
            }
         }
      }
      result.add_child("packages", allData);


      return CJsonResult::GenerateSuccess(result);
   }
   else
      return CJsonResult::GenerateError(widgetPath + " is not a valid directory.");
}
