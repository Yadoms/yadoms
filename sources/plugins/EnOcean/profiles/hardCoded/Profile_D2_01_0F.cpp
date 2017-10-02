#include "stdafx.h"
#include "Profile_D2_01_0F.h"
#include "Profile_D2_01_Common.h"

CProfile_D2_01_0F::CProfile_D2_01_0F(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_channel(boost::make_shared<yApi::historization::CSwitch>("Channel", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_channel})
{
}

CProfile_D2_01_0F::~CProfile_D2_01_0F()
{
}

const std::string& CProfile_D2_01_0F::profile() const
{
   static const std::string profile("D2-01-0F");
   return profile;
}

const std::string& CProfile_D2_01_0F::title() const
{
   static const std::string title("Slot-in module with 1 channel, no metering capabilities");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0F::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_01_0F::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   CProfile_D2_01_Common::sendActuatorStatusQuery(messageHandler,
                                                  senderId,
                                                  m_deviceId,
                                                  CProfile_D2_01_Common::kOutputChannel1);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0F::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   return CProfile_D2_01_Common::extractActuatorStatusResponse(rorg,
                                                               data,
                                                               m_channel,
                                                               CProfile_D2_01_Common::noChannel2,
                                                               CProfile_D2_01_Common::noDimmable,
                                                               CProfile_D2_01_Common::noPowerFailure,
                                                               CProfile_D2_01_Common::noOverCurrent);
}

void CProfile_D2_01_0F::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (keyword != m_channel->getKeyword())
      return;

   m_channel->setCommand(commandBody);

   CProfile_D2_01_Common::sendActuatorSetOutputCommandSwitching(messageHandler,
                                                                senderId,
                                                                m_deviceId,
                                                                CProfile_D2_01_Common::kOutputChannel1,
                                                                m_channel->get());
}

void CProfile_D2_01_0F::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   auto localControl = deviceConfiguration.get<std::string>("localControl") == "enable";
   auto taughtInAllDevices = deviceConfiguration.get<std::string>("taughtIn") == "allDevices";
   auto userInterfaceDayMode = deviceConfiguration.get<std::string>("userInterfaceMode") == "dayMode";
   auto defaultState = deviceConfiguration.get<CProfile_D2_01_Common::EDefaultState>("defaultState");
   auto connectedSwitchsType = deviceConfiguration.get<CProfile_D2_01_Common::EConnectedSwitchsType>("connectedSwitchsType");
   auto switchingStateToggle = deviceConfiguration.get<std::string>("switchingState") == "tooggle";
   auto autoOffTimerValue = deviceConfiguration.get<bool>("autoOffTimer.checkbox")
                               ? deviceConfiguration.get<double>("autoOffTimer.content.value")
                               : 0;
   auto delayOffTimer = deviceConfiguration.get<bool>("delayOffTimer.checkbox")
                           ? deviceConfiguration.get<double>("delayOffTimer.content.value")
                           : 0;

   CProfile_D2_01_Common::sendActuatorSetLocalCommand(messageHandler,
                                                      senderId,
                                                      m_deviceId,
                                                      CProfile_D2_01_Common::kOutputChannel1,
                                                      localControl,
                                                      taughtInAllDevices,
                                                      userInterfaceDayMode,
                                                      false,
                                                      defaultState,
                                                      0.0,
                                                      0.0,
                                                      0.0);

   CProfile_D2_01_Common::sendActuatorSetExternalInterfaceSettingsCommand(messageHandler,
                                                                          senderId,
                                                                          m_deviceId,
                                                                          CProfile_D2_01_Common::kOutputChannel1,
                                                                          connectedSwitchsType,
                                                                          autoOffTimerValue,
                                                                          delayOffTimer,
                                                                          switchingStateToggle);
}
