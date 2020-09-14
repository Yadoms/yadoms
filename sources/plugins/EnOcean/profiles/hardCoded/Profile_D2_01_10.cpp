#include "stdafx.h"
#include "Profile_D2_01_10.h"
#include "Profile_D2_01_Common.h"

CProfile_D2_01_10::CProfile_D2_01_10(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_channel1(boost::make_shared<yApi::historization::CSwitch>("Channel 1", yApi::EKeywordAccessMode::kGetSet)),
     m_channel2(boost::make_shared<yApi::historization::CSwitch>("Channel 2", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_channel1 , m_channel2})
{
}

const std::string& CProfile_D2_01_10::profile() const
{
   static const std::string profile("D2-01-10");
   return profile;
}

const std::string& CProfile_D2_01_10::title() const
{
   static const std::string title("Electronic switch (2 channels) with energy measurement and local control");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_10::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_01_10::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_01_Common::sendActuatorStatusQuery(messageHandler,
                                                  senderId,
                                                  m_deviceId,
                                                  CProfile_D2_01_Common::kAllOutputChannels);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_10::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   return CProfile_D2_01_Common::extractActuatorStatusResponse2Channels(rorg,
                                                               data,
                                                               m_channel1,
                                                               m_channel2,
                                                               CProfile_D2_01_Common::noDimmable,
                                                               CProfile_D2_01_Common::noPowerFailure,
                                                               CProfile_D2_01_Common::noOverCurrent);
}

void CProfile_D2_01_10::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   bool value;
   CProfile_D2_01_Common::EOutputChannel channel;
   if (keyword == m_channel1->getKeyword())
   {
      m_channel1->setCommand(commandBody);
      value = m_channel1->get();
      channel = CProfile_D2_01_Common::kOutputChannel1;
   }
   else if (keyword == m_channel2->getKeyword())
   {
      m_channel2->setCommand(commandBody);
      value = m_channel2->get();
      channel = CProfile_D2_01_Common::kOutputChannel2;
   }
   else
      return;

   CProfile_D2_01_Common::sendActuatorSetOutputCommandSwitching(messageHandler,
                                                                senderId,
                                                                m_deviceId,
                                                                channel,
                                                                value);
}

void CProfile_D2_01_10::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}

