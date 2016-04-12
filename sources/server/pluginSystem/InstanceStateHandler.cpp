#include "stdafx.h"
#include <shared/Log.h>
#include "InstanceStateHandler.h"

namespace pluginSystem
{
   CInstanceStateHandler::CInstanceStateHandler(boost::shared_ptr<database::IPluginRequester> pluginRequester,
                                                boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
                                                boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
                                                int instanceId,
                                                int normalStopEventId,
                                                int abnormalStopEventId)
      : m_pluginRequester(pluginRequester),
        m_eventLogger(eventLogger),
        m_managerEventHandler(managerEventHandler),
        m_instanceId(instanceId),
        m_normalStopEventId(normalStopEventId),
        m_abnormalStopEventId(abnormalStopEventId)
   {
   }

   CInstanceStateHandler::~CInstanceStateHandler()
   {
   }

   void CInstanceStateHandler::onEndOfProcess(int returnCode, const std::string& error)
   {
      if (returnCode == 0)
         signalNormalStop();
      else
         signalError(error);
   }

   void CInstanceStateHandler::signalNormalStop() const
   {
      YADOMS_LOG(information) << "Stop plugin instance #" << m_instanceId;

      // Signal the stop to asynchronously remove from running instances list
      m_managerEventHandler->postEvent(m_normalStopEventId, m_instanceId);
   }

   void CInstanceStateHandler::signalError(const std::string& error)
   {
      // Signal error
      YADOMS_LOG(error) << error;
      m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailed, m_pluginRequester->getInstance(m_instanceId)->DisplayName(), error);

      // Signal the abnormal stop to asynchronously remove from list
      m_managerEventHandler->postEvent(m_abnormalStopEventId, std::pair<int, std::string>(m_instanceId, error));
   }

   void CInstanceStateHandler::signalStartError(const std::string& error)
   {
      // Signal error
      YADOMS_LOG(error) << error;
      m_eventLogger->addEvent(database::entities::ESystemEventCode::kRuleFailed, (boost::format("Plugin instance #%1%") % m_instanceId).str(), error);

      // Signal the abnormal stop to asynchronously remove from list
      m_managerEventHandler->postEvent(m_abnormalStopEventId, std::pair<int, std::string>(m_instanceId, error));
   }
} // namespace pluginSystem	
	
	