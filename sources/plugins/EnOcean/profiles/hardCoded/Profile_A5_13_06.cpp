#include "stdafx.h"
#include "Profile_A5_13_06.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"
#include <algorithm>

CProfile_A5_13_06::CProfile_A5_13_06(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_latitude(boost::make_shared<yApi::historization::CDirectionDouble>("Latitude")),
     m_longitude(boost::make_shared<yApi::historization::CDirectionDouble>("Longitude")),
     m_historizers({ m_latitude, m_longitude })
{
}

const std::string& CProfile_A5_13_06::profile() const
{
   static const std::string Profile("A5-13-06");
   return Profile;
}

const std::string& CProfile_A5_13_06::title() const
{
   static const std::string Title(
      "Environmental Applications - Geographic Position Exchange");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_06::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_13_06::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_06::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_latitude->set(static_cast<double>(bitset_extract(data, 0, 4) << 8 || bitset_extract(data, 8, 8)) * 180.0 / 4096.0 - 90.0);
   m_longitude->set(static_cast<double>(bitset_extract(data, 4, 4) << 8 || bitset_extract(data, 16, 8)) * 180.0 / 4096.0 - 90.0);

   return m_historizers;
}

void CProfile_A5_13_06::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_13_06::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
