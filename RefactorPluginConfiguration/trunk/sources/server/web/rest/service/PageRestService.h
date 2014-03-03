#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CPageRestService : public IRestService
   {
   public:
      CPageRestService(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CPageRestService();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      static const std::string & getRestKeyword();
      web::rest::json::CJson getOnePage(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson getAllPages(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  
      web::rest::json::CJson getPageWidget(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   

      web::rest::json::CJson addPage(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson updatePage(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson updateAllPages(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson deletePage(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson deleteAllPages(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   


      web::rest::json::CJson addWidgetForPage(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson replaceAllWidgetsForPage(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   
      web::rest::json::CJson deleteAllWidgetsForPage(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);   

      web::rest::json::CJson transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
