#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "pluginSystem/Manager.h"

namespace web { namespace rest { namespace service {

   class CPlugin : public IRestService
   {
   public:
      CPlugin(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<pluginSystem::CManager> pluginManager);
      virtual ~CPlugin();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();
      web::rest::json::CJson getOnePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson getAllPluginsInstance(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson getAllAvailablePlugins(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson createPlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson updatePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson deletePlugin(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson deleteAllPlugins(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

      web::rest::json::CJson getInstanceStatus(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson startInstance(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson stopInstance(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   


      web::rest::json::CJson transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      boost::shared_ptr<pluginSystem::CManager> m_pluginManager;
      std::string m_restKeyword;
   };



} //namespace service
} //namespace rest
} //namespace web 
