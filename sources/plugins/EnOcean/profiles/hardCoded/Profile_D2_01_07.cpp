#include "stdafx.h"
#include "Profile_D2_01_07.h"
#include "Profile_D2_01_Common.h"

CProfile_D2_01_07::CProfile_D2_01_07(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_channel(boost::make_shared<yApi::historization::CSwitch>("Channel", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_channel})
{
}

CProfile_D2_01_07::~CProfile_D2_01_07()
{
}

const std::string& CProfile_D2_01_07::profile() const
{
   static const std::string profile("D2-01-07");
   return profile;
}

const std::string& CProfile_D2_01_07::title() const
{
   static const std::string title("Electronic switch");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_07::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_07::states(unsigned char rorg,
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
                                                               CProfile_D2_01_Common::noPowerFailure);
}

void CProfile_D2_01_07::sendCommand(const std::string& keyword,
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

void CProfile_D2_01_07::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
