#include "stdafx.h"
#include "Profile_A5_13_05.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_13_05::CProfile_A5_13_05(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_elevation(boost::make_shared<yApi::historization::CDirection>("Elevation")),
     m_azimuth(boost::make_shared<yApi::historization::CDirection>("Azimuth")),
     m_historizers({m_elevation, m_azimuth})
{
}

const std::string& CProfile_A5_13_05::profile() const
{
   static const std::string Profile("A5-13-05");
   return Profile;
}

const std::string& CProfile_A5_13_05::title() const
{
   static const std::string Title("Environmental Applications - Direction Exchange");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_05::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_13_05::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_05::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (bitset_extract(data, 24, 4) != 5)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : identifier=" << bitset_extract(data, 24, 4);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_elevation->set(bitset_extract(data, 0, 8) - 90);
   m_azimuth->set(bitset_extract(data, 15, 9));

   return m_historizers;
}

void CProfile_A5_13_05::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_13_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
