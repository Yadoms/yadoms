#include "stdafx.h"
#include "EventLoggerRestService.h"
#include "json/JsonSerializers.h"
#include "json/JsonCollectionSerializer.h"
#include "RestDispatcherHelpers.hpp"
#include "shared/Log.h"


CEventLoggerRestService::CEventLoggerRestService(boost::shared_ptr<IDataProvider> dataProvider)
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
