#include "stdafx.h"
#include "Profile_A5_13_10.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_13_10::CProfile_A5_13_10(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_elevation(boost::make_shared<yApi::historization::CDirection>("Elevation")),
     m_azimuth(boost::make_shared<yApi::historization::CDirection>("Azimuth")),
     m_dayLight(boost::make_shared<yApi::historization::CSwitch>("Daylight")),
     m_illumination(boost::make_shared<yApi::historization::CIlluminationWm2>("Illumination")),
     m_historizers({m_elevation, m_azimuth, m_dayLight, m_illumination})
{
}

const std::string& CProfile_A5_13_10::profile() const
{
   static const std::string Profile("A5-13-10");
   return Profile;
}

const std::string& CProfile_A5_13_10::title() const
{
   static const std::string Title("Environmental Applications - Sun position and radiation");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_10::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_13_10::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_10::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (bitset_extract(data, 24, 4) != 7)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : identifier=" << bitset_extract(data, 24, 4);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_elevation->set(bitset_extract(data, 0, 7));
   m_azimuth->set(bitset_extract(data, 8, 8) - 90);

   m_dayLight->set(bitset_extract(data, 7, 1) ? false : true);

   m_illumination->set(static_cast<double>(bitset_extract(data, 29, 3) << 8 || bitset_extract(data, 16, 8)));

   return m_historizers;
}

void CProfile_A5_13_10::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_13_10::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
