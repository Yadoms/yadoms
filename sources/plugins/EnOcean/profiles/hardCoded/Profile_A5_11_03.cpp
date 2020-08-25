#include "stdafx.h"
#include "Profile_A5_11_03.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_11_03::CProfile_A5_11_03(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_blindPosition(boost::make_shared<yApi::historization::CDimmable>("BlindPosition")),
     m_angle(boost::make_shared<yApi::historization::CDirection>("Angle")),
     m_errorState(boost::make_shared<yApi::historization::CSwitch>("ErrorState")),
     m_errorMessage(boost::make_shared<yApi::historization::CText>("ErrorMessage")),
     m_curtain(boost::make_shared<yApi::historization::CCurtain>("BlindCurrentPosition")),
     m_currentMovement(boost::make_shared<yApi::historization::CCurtain>("BlindCurrentMovement")),
     m_serviceMode(boost::make_shared<yApi::historization::CSwitch>("ServiceModeActivated")),
     m_inverseMode(boost::make_shared<yApi::historization::CSwitch>("InverseMode")),
     m_historizers({
        m_blindPosition, m_angle, m_errorState, m_errorMessage, m_curtain, m_currentMovement,
        m_serviceMode, m_inverseMode
     })
{
}

const std::string& CProfile_A5_11_03::profile() const
{
   static const std::string Profile("A5-11-03");
   return Profile;
}

const std::string& CProfile_A5_11_03::title() const
{
   static const std::string Title(
      "Controller Status - Blind Status");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_03::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_11_03::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_03::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   if (bitset_extract(data, 16, 1))
   {
      m_blindPosition->set(static_cast<int>(bitset_extract(data, 0, 8)));
      historizers.push_back(m_blindPosition);
   }

   if (bitset_extract(data, 17, 1))
   {
      const auto absAngle = static_cast<int>(bitset_extract(data, 9, 7));
      m_angle->set(bitset_extract(data, 8, 1) ? -absAngle : absAngle);
      historizers.push_back(m_angle);
   }

   switch (bitset_extract(data, 18, 2))
   {
   case 0:
      m_errorState->set(false);
      m_errorMessage->set(std::string());
      historizers.push_back(m_errorState);
      historizers.push_back(m_errorMessage);
      break;
   case 1:
      m_errorState->set(true);
      m_errorMessage->set("End-positions are not configured");
      historizers.push_back(m_errorState);
      historizers.push_back(m_errorMessage);
      break;
   case 2:
      m_errorState->set(true);
      m_errorMessage->set("Internal Failure");
      historizers.push_back(m_errorState);
      historizers.push_back(m_errorMessage);
      break;
   default:
      break;
   }

   switch (bitset_extract(data, 20, 2))
   {
   case 1:
      m_curtain->set(yApi::historization::ECurtainCommand::kStop);
      historizers.push_back(m_curtain);
      break;
   case 2:
      m_curtain->set(yApi::historization::ECurtainCommand::kOpen);
      historizers.push_back(m_curtain);
      break;
   case 3:
      m_curtain->set(yApi::historization::ECurtainCommand::kClose);
      historizers.push_back(m_curtain);
      break;
   default:
      break;
   }

   switch (bitset_extract(data, 22, 2))
   {
   case 1:
      m_currentMovement->set(yApi::historization::ECurtainCommand::kStop);
      historizers.push_back(m_currentMovement);
      break;
   case 2:
      m_currentMovement->set(yApi::historization::ECurtainCommand::kOpen);
      historizers.push_back(m_currentMovement);
      break;
   case 3:
      m_currentMovement->set(yApi::historization::ECurtainCommand::kClose);
      historizers.push_back(m_currentMovement);
      break;
   default:
      break;
   }

   m_serviceMode->set(bitset_extract(status, 24, 1) ? true : false);
   historizers.push_back(m_serviceMode);

   m_inverseMode->set(bitset_extract(status, 25, 1) ? true : false);
   historizers.push_back(m_inverseMode);

   return historizers;
}

void CProfile_A5_11_03::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_11_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
