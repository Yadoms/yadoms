#include "stdafx.h"
#include "Profile_F6_10_01.h"
#include "../bitsetHelpers.hpp"
#include <shared/Log.h>

CProfile_F6_10_01::CProfile_F6_10_01(const std::string& deviceId,
   boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
   m_deviceId(deviceId),
   m_movement(boost::make_shared<specificHistorizers::CMechanicalHandleMovement>("Movement")),
   m_historizers({ m_movement })
{
}

const std::string& CProfile_F6_10_01::profile() const
{
   static const std::string profile("F6-10-01");
   return profile;
}

const std::string& CProfile_F6_10_01::title() const
{
   static const std::string title("Mechanical handle - Windows handle ERP2");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_10_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_F6_10_01::readInitialState(const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_10_01::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   const auto movement = bitset_extract(status, 4, 4);

   if ((movement & BOOST_BINARY(00001101)) == BOOST_BINARY(00001100))
      m_movement->set(specificHistorizers::EMechanicalHandleMovement::kToHorizontal);
   else if ((movement & BOOST_BINARY(00001111)) == BOOST_BINARY(00001111))
      m_movement->set(specificHistorizers::EMechanicalHandleMovement::kToDown);
   else if ((movement & BOOST_BINARY(00001111)) == BOOST_BINARY(00001101))
      m_movement->set(specificHistorizers::EMechanicalHandleMovement::kToUp);
   else
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : movement=" << movement;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return m_historizers;
}

void CProfile_F6_10_01::sendCommand(const std::string& keyword,
   const std::string& commandBody,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_F6_10_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
