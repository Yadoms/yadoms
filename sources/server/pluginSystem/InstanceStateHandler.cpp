#include "stdafx.h"
#include <shared/Log.h>
#include "InstanceStateHandler.h"

namespace pluginSystem
{

CInstanceStateHandler::CInstanceStateHandler(boost::shared_ptr<database::IPluginRequester> dbRequester,
   boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
   boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
   int instanceId)
   :m_pluginRequester(dbRequester), m_eventLogger(eventLogger), m_managerEventHandler(managerEventHandler), m_instanceId(instanceId)
{
}

CInstanceStateHandler::~CInstanceStateHandler()
{
}

void CInstanceStateHandler::signalNormalStop()
{
   YADOMS_LOG(information) << "Stop plugin instance #" << m_instanceId;

   // Signal the stop to asynchronously remove from running instances list
   m_managerEventHandler->postEvent(kStopped, m_instanceId);
}

void CInstanceStateHandler::signalError(const std::string& error)
{
   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailed, m_pluginRequester->getInstance(m_instanceId)->DisplayName(), error);

   // Signal the abnormal stop to asynchronously remove from list
   m_managerEventHandler->postEvent(kAbnormalStopped, std::pair<int, std::string>(m_instanceId, error));
}

void CInstanceStateHandler::signalStartError(const std::string& error)
{
   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailed, "Automation rules", error);

   // Signal the abnormal stop to asynchronously remove from list
   m_managerEventHandler->postEvent(kAbnormalStopped, std::pair<int, std::string>(m_instanceId, error));
}

} // namespace pluginSystem	
	
	