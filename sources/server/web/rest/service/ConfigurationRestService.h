#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CConfigurationRestService : public IRestService
   {
   public:
      CConfigurationRestService(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CConfigurationRestService();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation


      static const std::string & getRestKeyword();

   public:
      web::rest::json::CJson getConfiguration(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson getSectionConfigurations(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson getAllConfigurations(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);

      web::rest::json::CJson createOneConfiguration(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson updateOneConfiguration(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson updateAllConfigurations(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson deleteOneConfiguration(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);
      web::rest::json::CJson deleteAllConfigurations(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
