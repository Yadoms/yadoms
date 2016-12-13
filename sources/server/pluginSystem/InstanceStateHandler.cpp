#include "stdafx.h"
#include <shared/Log.h>
#include "InstanceStateHandler.h"

namespace pluginSystem
{
   CInstanceStateHandler::CInstanceStateHandler(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                                                boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                                boost::shared_ptr<database::IPluginRequester> pluginRequester,
                                                boost::shared_ptr<IQualifier> qualifier,
                                                boost::shared_ptr<database::IPluginEventLoggerRequester> pluginEventLoggerRequester,
                                                boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
                                                boost::shared_ptr<IInstanceStoppedListener> instanceStoppedListener,
                                                boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
                                                boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordManager)
      : m_instanceData(instanceData),
        m_pluginInformation(pluginInformation),
        m_pluginRequester(pluginRequester),
        m_qualifier(qualifier),
        m_pluginEventLoggerRequester(pluginEventLoggerRequester),
        m_acquisitionHistorizer(acquisitionHistorizer),
        m_instanceStoppedListener(instanceStoppedListener),
        m_deviceManager(deviceManager),
        m_keywordDataAccessLayer(keywordManager),
        m_pluginStateKeywordId(pluginStateKeywordId()),
        m_pluginStateMessageIdKeywordId(pluginStateMessageIdKeywordId())
   {
   }

   CInstanceStateHandler::~CInstanceStateHandler()
   {
   }

   void CInstanceStateHandler::onStart()
   {
      m_qualifier->signalLoad(m_pluginInformation);
      setState(shared::plugin::yPluginApi::historization::EPluginState::kRunning);
   }

   void CInstanceStateHandler::onFinish(int returnCode, const std::string& error)
   {
      if (returnCode == 0)
         signalNormalStop();
      else
         signalError(returnCode, error);
   }

   void CInstanceStateHandler::signalNormalStop()
   {
      YADOMS_LOG(information) << "Stop plugin instance #" << m_instanceData->Id();

      setState(shared::plugin::yPluginApi::historization::EPluginState::kStopped);

      m_qualifier->signalUnload(m_pluginInformation);

      m_instanceStoppedListener->onStopped(m_instanceData->Id());
   }

   void CInstanceStateHandler::signalError(int returnCode,
                                           const std::string& error)
   {
      YADOMS_LOG(error) << "Stop plugin instance #" << m_instanceData->Id() << " because of error : " << error << ", return code = " << returnCode;

      setState(shared::plugin::yPluginApi::historization::EPluginState::kError, error);

      m_qualifier->signalCrash(m_pluginInformation, error);

      m_instanceStoppedListener->onStopped(m_instanceData->Id());
   }

   void CInstanceStateHandler::signalStartError(const std::string& error)
   {
      YADOMS_LOG(error) << "Fail to start plugin instance #" << m_instanceData->Id() << " : " << error;

      setState(shared::plugin::yPluginApi::historization::EPluginState::kError, error);

      m_instanceStoppedListener->onStopped(m_instanceData->Id());
   }

   void CInstanceStateHandler::setState(const shared::plugin::yPluginApi::historization::EPluginState& state, const std::string & customMessageId, const std::string & customMessageData)
   {
      if (!customMessageId.empty() && (state != shared::plugin::yPluginApi::historization::EPluginState::kCustom && state != shared::plugin::yPluginApi::historization::EPluginState::kError))
         YADOMS_LOG(warning) << "Custom message ID \"" << customMessageId << "\" will be ignored as state is " << state.toString();

      m_pluginStateKeyword->set(state);
      m_pluginStateMessage->setMessage(customMessageId, customMessageData);
      m_acquisitionHistorizer->saveData(m_pluginStateKeywordId, *m_pluginStateKeyword);
      m_acquisitionHistorizer->saveData(m_pluginStateMessageIdKeywordId, *m_pluginStateMessage);

      switch (state)
      {
      case shared::plugin::yPluginApi::historization::EPluginState::kErrorValue:
         recordPluginEvent(kError, (boost::format("error (%1%)") % customMessageId).str());
         break;
      case shared::plugin::yPluginApi::historization::EPluginState::kStoppedValue:
         recordPluginEvent(kInfo, "stopped");
         break;
      case shared::plugin::yPluginApi::historization::EPluginState::kRunningValue:
         recordPluginEvent(kInfo, "started");
         break;
      case shared::plugin::yPluginApi::historization::EPluginState::kCustomValue:
         recordPluginEvent(kInfo, (boost::format("custom event (%1%)") % customMessageId).str()); 
         break;
      default: break;
      }
   }

   int CInstanceStateHandler::pluginStateDeviceId() const
   {
      static const std::string PluginStateDeviceName("pluginState");

      if (!m_deviceManager->deviceExists(m_instanceData->Id(), PluginStateDeviceName))
         m_deviceManager->createDevice(m_instanceData->Id(), PluginStateDeviceName, m_instanceData->DisplayName() + " plugin state", "Plugin state", shared::CDataContainer::EmptyContainer);

      return m_deviceManager->getDeviceInPlugin(m_instanceData->Id(), PluginStateDeviceName)->Id();
   }

   int CInstanceStateHandler::pluginStateKeywordId()
   {
      static const std::string PluginStateKeywordName("state");

      if (!m_pluginStateKeyword)
      {
         m_pluginStateKeyword = boost::make_shared<shared::plugin::yPluginApi::historization::CPluginState>(PluginStateKeywordName);

         if (!m_keywordDataAccessLayer->keywordExists(pluginStateDeviceId(), PluginStateKeywordName))
            m_keywordDataAccessLayer->addKeyword(pluginStateDeviceId(), *m_pluginStateKeyword);
      }

      return m_keywordDataAccessLayer->getKeyword(pluginStateDeviceId(), PluginStateKeywordName)->Id();
   }

   int CInstanceStateHandler::pluginStateMessageIdKeywordId()
   {
      static const std::string PluginStateMessageIdKeywordName("customMessageId");

      if (!m_pluginStateMessage)
      {
         m_pluginStateMessage = boost::make_shared<shared::plugin::yPluginApi::historization::CPluginStateMessage>(PluginStateMessageIdKeywordName);

         if (!m_keywordDataAccessLayer->keywordExists(pluginStateDeviceId(), PluginStateMessageIdKeywordName))
            m_keywordDataAccessLayer->addKeyword(pluginStateDeviceId(), *m_pluginStateMessage);
      }

      return m_keywordDataAccessLayer->getKeyword(pluginStateDeviceId(), PluginStateMessageIdKeywordName)->Id();
   }

   void CInstanceStateHandler::recordPluginEvent(PluginEventSeverity severity, const std::string& message)
   {
      database::entities::EEventType evenType;
      switch (severity)
      {
      case kError: evenType = database::entities::EEventType::kError;
         break;
      case kInfo: evenType = database::entities::EEventType::kInfo;
         break;
      default:
         {
            YADOMS_LOG(warning) << "Unknown plugin event severity type " << severity;
            evenType = database::entities::EEventType::kInfo; // Set a default value
            break;
         }
      }

      m_pluginEventLoggerRequester->addEvent(m_pluginInformation->getType(),
                                             m_pluginInformation->getVersion(),
                                             m_pluginInformation->getReleaseType(),
                                             evenType,
                                             message);
   }

} // namespace pluginSystem	
	
	