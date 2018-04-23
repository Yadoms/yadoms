#include "stdafx.h"
#include "PluginStateHelper.h"


CPluginStateHelper::CPluginStateHelper(boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_lastInternalState(kStopped)
{
}

CPluginStateHelper::~CPluginStateHelper()
{
}

void CPluginStateHelper::set(EState internalState,
                             const std::string& customMessageId,
                             const std::map<std::string, std::string>& customMessageDataParams)
{
   switch (internalState)
   {
   case kStartPairing:      
      notifyServer(kStartPairing);
      // Don't update "last-" data
      return;
   case kStopPairing:
      // Restore previous state
      notifyServer(m_lastInternalState,
                   m_lastMessage,
                   m_lastMessageData);
      return;
   default:
      m_lastInternalState = internalState;
      m_lastMessage = customMessageId;
      m_lastMessageData = customMessageDataParams;
      notifyServer(m_lastInternalState,
                   m_lastMessage,
                   m_lastMessageData);
      break;
   }
}

void CPluginStateHelper::notifyServer(EState internalState,
                                      const std::string& customMessageId,
                                      const std::map<std::string, std::string>& customMessageDataParams) const
{
   switch (internalState)
   {
   case kStopped:
      m_api->setPluginState(yApi::historization::EPluginState::kStopped);
      break;
   case kRunning:
      m_api->setPluginState(yApi::historization::EPluginState::kRunning);
      break;
   case kError:
      m_api->setPluginState(yApi::historization::EPluginState::kError,
                            customMessageId,
                            customMessageDataParams);
      break;
   case kConnecting:
      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
      break;
   case kInitializing:
      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "initializing");
      break;
   case kConnectionLost:
      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "connectionLost");
      break;
   case kUpdateConfiguration:
      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
      break;
   case kFailToConfigure:
      m_api->setPluginState(yApi::historization::EPluginState::kError, "failToConfigure");
      break;
   case kUpdateFirmware:
      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "updateFirmware");
      break;
   case kStartPairing:
      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "startPairing");
      break;
   case kStopPairing:
      m_api->setPluginState(yApi::historization::EPluginState::kCustom, "startPairing");
      break;
   default:
      YADOMS_LOG(error) << "Invalid plugin state : " << internalState;
   }
}
