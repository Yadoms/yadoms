#include "stdafx.h"
#include "Profile_D2_05_00.h"
#include "Profile_D2_05_Common.h"

CProfile_D2_05_00::CProfile_D2_05_00(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_curtain(boost::make_shared<yApi::historization::CCurtain>("Curtain")),
     m_lockingMode(boost::make_shared<specificHistorizers::CBlindLockingMode>("lockingMode")),
     m_historizers({m_curtain, m_lockingMode})
{
}

const std::string& CProfile_D2_05_00::profile() const
{
   static const std::string profile("D2-05-00");
   return profile;
}

const std::string& CProfile_D2_05_00::title() const
{
   static const std::string title("Blinds control for position and angle");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_05_00::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_05_00::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_05_Common::sendQueryPositionAndAngle(messageHandler,
                                                    senderId,
                                                    m_deviceId);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_05_00::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler)
const
{
   return CProfile_D2_05_Common::extractReplyPositionAndAngleResponse(rorg,
                                                                      data,
                                                                      m_curtain,
                                                                      m_lockingMode);
}

void CProfile_D2_05_00::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (keyword == m_curtain->getKeyword())
   {
      m_curtain->setCommand(commandBody);

      if (m_curtain->get() == yApi::historization::ECurtainCommand::kStopValue)
      {
         CProfile_D2_05_Common::sendStop(messageHandler,
                                         senderId,
                                         m_deviceId);
         return;
      }
   }
   else if (keyword == m_lockingMode->getKeyword())
   {
      m_lockingMode->setCommand(commandBody);
   }
   else
   {
      return;
   }

   CProfile_D2_05_Common::sendGoToPositionAndAngle(messageHandler,
                                                   senderId,
                                                   m_deviceId,
                                                   m_curtain->get(),
                                                   m_lockingMode->get());
}

void CProfile_D2_05_00::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   const auto measuredDurationOfVerticalRunMs = deviceConfiguration.get<unsigned int>("measuredDurationOfVerticalRunMs");
   const auto measuredDurationOfRotationMs = deviceConfiguration.get<unsigned int>("measuredDurationOfRotationMs");
   const auto alarmAction = deviceConfiguration.get<CProfile_D2_05_Common::EAlarmAction>("alarmAction");

   CProfile_D2_05_Common::sendSetParameters(messageHandler,
                                            senderId,
                                            m_deviceId,
                                            measuredDurationOfVerticalRunMs,
                                            measuredDurationOfRotationMs,
                                            alarmAction);
}
