#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "web/rest/RestDispatcher.h"

namespace web { namespace rest { namespace service {

   class CWidget : public IRestService
   {
   public:
      CWidget(boost::shared_ptr<database::IDataProvider> dataProvider, const std::string & webServerPath);
      virtual ~CWidget();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation


      static const std::string & getRestKeyword();

   public:
      web::rest::json::CJson getOneWidget(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson getAllWidgets(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson addWidget(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson deleteOneWidget(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson deleteAllWidgets(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson replaceAllWidgets(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson updateOneWidget(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson findWidgetPackages(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);

      web::rest::json::CJson transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
      std::string m_webServerPath;
   };


} //namespace service
} //namespace rest
} //namespace web 
