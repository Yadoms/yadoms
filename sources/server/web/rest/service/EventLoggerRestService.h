#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CEventLoggerRestService : public IRestService
   {
   public:
      CEventLoggerRestService(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CEventLoggerRestService();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();


   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
