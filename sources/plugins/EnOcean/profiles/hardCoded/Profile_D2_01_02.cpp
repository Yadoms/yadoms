#include "stdafx.h"
#include "Profile_D2_01_02.h"
#include "Profile_D2_01_Common.h"

CProfile_D2_01_02::CProfile_D2_01_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_dimmer(boost::make_shared<yApi::historization::CDimmable>("Dimmer", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_dimmer})
{
}

CProfile_D2_01_02::~CProfile_D2_01_02()
{
}

const std::string& CProfile_D2_01_02::profile() const
{
   static const std::string profile("D2-01-02");
   return profile;
}

const std::string& CProfile_D2_01_02::title() const
{
   static const std::string title("Electronic dimmer with local Control");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_02::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_01_02::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_01_Common::sendActuatorStatusQuery(messageHandler,
                                                  senderId,
                                                  m_deviceId,
                                                  CProfile_D2_01_Common::kOutputChannel1);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_02::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   return CProfile_D2_01_Common::extractActuatorStatusResponse(rorg,
                                                               data,
                                                               CProfile_D2_01_Common::noChannel1,
                                                               CProfile_D2_01_Common::noChannel2,
                                                               m_dimmer,
                                                               CProfile_D2_01_Common::noPowerFailure,
                                                               CProfile_D2_01_Common::noOverCurrent);
}

void CProfile_D2_01_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (keyword != m_dimmer->getKeyword())
      return;

   m_dimmer->setCommand(commandBody);

   CProfile_D2_01_Common::sendActuatorSetOutputCommandDimming(messageHandler,
                                                              senderId,
                                                              m_deviceId,
                                                              CProfile_D2_01_Common::kOutputChannel1,
                                                              specificHistorizers::EDimmerMode::kSwitchToValue,
                                                              m_dimmer->get());
}

void CProfile_D2_01_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}

