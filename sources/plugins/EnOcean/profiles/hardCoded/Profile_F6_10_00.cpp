#include "stdafx.h"
#include "Profile_F6_10_00.h"
#include "../bitsetHelpers.hpp"
#include <shared/Log.h>

CProfile_F6_10_00::CProfile_F6_10_00(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_movement(boost::make_shared<specificHistorizers::CMechanicalHandleMovement>("Movement")),
     m_historizers({m_movement})
{
}

const std::string& CProfile_F6_10_00::profile() const
{
   static const std::string profile("F6-10-00");
   return profile;
}

const std::string& CProfile_F6_10_00::title() const
{
   static const std::string title("Mechanical handle - Windows handle");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_10_00::allHistorizers() const
{
   return m_historizers;
}

void CProfile_F6_10_00::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_F6_10_00::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   const auto T21 = bitset_extract(status, 2, 1) ? true : false;
   const auto NU = bitset_extract(status, 3, 1) ? true : false;
   const auto movement = bitset_extract(status, 0, 8);

   if (!T21)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : T21=" << (T21 ? "1" : "0");
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }
   if (NU)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : NU=" << (NU ? "1" : "0");
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   if ((movement & BOOST_BINARY(11010000)) == BOOST_BINARY(11000000))
      m_movement->set(specificHistorizers::EMechanicalHandleMovement::kToHorizontal);
   else if ((movement & BOOST_BINARY(11110000)) == BOOST_BINARY(11110000))
      m_movement->set(specificHistorizers::EMechanicalHandleMovement::kToDown);
   else if ((movement & BOOST_BINARY(11110000)) == BOOST_BINARY(11010000))
      m_movement->set(specificHistorizers::EMechanicalHandleMovement::kToUp);
   else
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : movement=" << movement;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return m_historizers;
}

void CProfile_F6_10_00::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_F6_10_00::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
