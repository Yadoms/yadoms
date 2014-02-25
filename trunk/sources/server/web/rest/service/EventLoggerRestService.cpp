#include "stdafx.h"
#include "EventLoggerRestService.h"
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"

namespace web { namespace rest { namespace service {

   CEventLoggerRestService::CEventLoggerRestService(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider), m_restKeyword("EventLogger")
   {
   }


   CEventLoggerRestService::~CEventLoggerRestService()
   {
   }

   const std::string & CEventLoggerRestService::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CEventLoggerRestService::configureDispatcher(CRestDispatcher & dispatcher)
   {
   }


} //namespace service
} //namespace rest
} //namespace web 
