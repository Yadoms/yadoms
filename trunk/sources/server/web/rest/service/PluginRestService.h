#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CPluginRestService : public IRestService
   {
   public:
      CPluginRestService(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CPluginRestService();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();
      web::rest::json::CJson getOnePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson getAllPlugins(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson createPlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson updatePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson deletePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson deleteAllPlugins(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

      web::rest::json::CJson transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      std::string m_restKeyword;
   };



} //namespace service
} //namespace rest
} //namespace web 
