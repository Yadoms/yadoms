#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CPluginEventLogger : public IRestService
   {
   public:
      CPluginEventLogger(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CPluginEventLogger();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();

      shared::CDataContainer getLogsForPluginName(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  
      shared::CDataContainer getLogsForPluginNameFromDate(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      std::string m_restKeyword;
   };



} //namespace service
} //namespace rest
} //namespace web 

