#include "stdafx.h"
#include <shared/Log.h>
#include "PluginStateHandler.h"

namespace pluginSystem
{

CPluginStateHandler::CPluginStateHandler(boost::shared_ptr<database::IPluginRequester> dbRequester,
   boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
   boost::shared_ptr<shared::event::CEventHandler> pluginEventHandler)
   :m_pluginRequester(dbRequester), m_eventLogger(eventLogger), m_pluginEventHandler(pluginEventHandler)
{
}

CPluginStateHandler::~CPluginStateHandler()
{
}

void CPluginStateHandler::signalNormalStop(int instanceId)
{
   YADOMS_LOG(information) << "Stop plugin #" << instanceId;

   // Signal the stop to asynchronously remove from running instances list
   m_pluginEventHandler->postEvent(kStopped, instanceId);
}

void CPluginStateHandler::signalError(int instanceId, const std::string& error)
{
   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLogger->addEvent(database::entities::ESystemEventCode::kPluginCrash, m_pluginRequester->getInstance(instanceId)->DisplayName(), error);

   // Signal the abnormal stop to asynchronously remove from list
   m_pluginEventHandler->postEvent(kAbnormalStopped, std::pair<int, std::string>(instanceId, error));
}

void CPluginStateHandler::signalStartError(int instanceId, const std::string& error)
{
   // Signal error
   YADOMS_LOG(error) << error;
   m_eventLogger->addEvent(database::entities::ESystemEventCode::kPluginCrash, "plugin", error);

   // Signal the abnormal stop to asynchronously remove from list
   m_pluginEventHandler->postEvent(kAbnormalStopped, std::pair<int, std::string>(instanceId, error));
}

} // namespace pluginSystem	
	
	