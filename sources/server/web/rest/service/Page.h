#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CPage : public IRestService
   {
   public:
      explicit CPage(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CPage();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      static const std::string & getRestKeyword();
      shared::CDataContainer getOnePage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   
      shared::CDataContainer getAllPages(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  
      shared::CDataContainer getPageWidget(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   

      shared::CDataContainer addPage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   
      shared::CDataContainer updatePage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   
      shared::CDataContainer updateAllPages(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   
      shared::CDataContainer deletePage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   
      shared::CDataContainer deleteAllPages(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   


      shared::CDataContainer addWidgetForPage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   
      shared::CDataContainer replaceAllWidgetsForPage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   
      shared::CDataContainer deleteAllWidgetsForPage(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);   

      shared::CDataContainer transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      static std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
