#include "stdafx.h"
#include "Profile_D2_01_0A.h"
#include "Profile_D2_01_Common.h"

CProfile_D2_01_0A::CProfile_D2_01_0A(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_channel(boost::make_shared<yApi::historization::CSwitch>("Channel", yApi::EKeywordAccessMode::kGetSet)),
     m_powerFailure(boost::make_shared<yApi::historization::CSwitch>("Power failure", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_channel, m_powerFailure})
{
}

const std::string& CProfile_D2_01_0A::profile() const
{
   static const std::string Profile("D2-01-0A");
   return Profile;
}

const std::string& CProfile_D2_01_0A::title() const
{
   static const std::string Title(R"(Electronic switch with local control)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0A::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_01_0A::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_01_Common::sendActuatorStatusQuery(messageHandler,
                                                  senderId,
                                                  m_deviceId,
                                                  CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0A::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   return CProfile_D2_01_Common::extractActuatorStatusResponse(rorg,
                                                               data,
                                                               m_channel,
                                                               CProfile_D2_01_Common::NoDimmable,
                                                               m_powerFailure,
                                                               CProfile_D2_01_Common::NoOverCurrent);
}

void CProfile_D2_01_0A::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) {
   if (keyword != m_channel->getKeyword())
      return;

   m_channel->setCommand(commandBody);

   CProfile_D2_01_Common::sendActuatorSetOutputCommandSwitching(messageHandler,
                                                                senderId,
                                                                m_deviceId,
                                                                CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                                m_channel->get());
}

void CProfile_D2_01_0A::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler)
{
   const auto localControl = deviceConfiguration.get<std::string>("localControl") == "enable";
   const auto taughtInAllDevices = deviceConfiguration.get<std::string>("taughtIn") == "allDevices";
   const auto userInterfaceDayMode = deviceConfiguration.get<std::string>("userInterfaceMode") == "dayMode";
   const auto defaultState = deviceConfiguration.get<CProfile_D2_01_Common::EDefaultState>("defaultState");
   const auto powerFailureDetection = deviceConfiguration.get<std::string>("powerFailureDetection") == "enable";

   CProfile_D2_01_Common::sendActuatorSetLocalCommand(messageHandler,
                                                      senderId,
                                                      m_deviceId,
                                                      CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                      localControl,
                                                      taughtInAllDevices,
                                                      userInterfaceDayMode,
                                                      powerFailureDetection,
                                                      defaultState,
                                                      0.0,
                                                      0.0,
                                                      0.0);
}
