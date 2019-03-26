#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CPluginEventLogger : public IRestService
   {
   public:
      explicit CPluginEventLogger(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CPluginEventLogger();

   public:
      // IRestService implementation
      void configureDispatcher(CRestDispatcher & dispatcher) override;
      // [END] IRestService implementation

      const std::string & getRestKeyword();

      boost::shared_ptr<shared::serialization::IDataSerializable> getLogsForPluginName(const std::vector<std::string> & parameters, const std::string & requestContent);
      boost::shared_ptr<shared::serialization::IDataSerializable> getLogsForPluginNameFromDate(const std::vector<std::string> & parameters, const std::string & requestContent);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      std::string m_restKeyword;
   };



} //namespace service
} //namespace rest
} //namespace web 

