#include "stdafx.h"
#include "Profile_A5_13_01.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_13_01::CProfile_A5_13_01(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_dawn(boost::make_shared<yApi::historization::CIllumination>("Dawn sensor")),
     m_temperature(boost::make_shared<yApi::historization::CTemperature>("Outdoor temperature")),
     m_windSpeed(boost::make_shared<yApi::historization::CSpeed>("Wind speed")),
     m_dayLight(boost::make_shared<yApi::historization::CSwitch>("Daylight")),
     m_rain(boost::make_shared<yApi::historization::CSwitch>("Rain")),
     m_historizers({m_dawn, m_temperature, m_windSpeed, m_dayLight, m_rain}),
     m_subProfile_A5_13_02(deviceId, api),
     m_subProfile_A5_13_06(deviceId, api)
{
   const auto subProfile_A5_13_02_Historizers = m_subProfile_A5_13_02.allHistorizers();
   m_historizers.insert(m_historizers.end(), subProfile_A5_13_02_Historizers.begin(),
                        subProfile_A5_13_02_Historizers.end());

   const auto subProfile_A5_13_06_Historizers = m_subProfile_A5_13_06.allHistorizers();
   m_historizers.insert(m_historizers.end(), subProfile_A5_13_06_Historizers.begin(),
                        subProfile_A5_13_06_Historizers.end());
}

const std::string& CProfile_A5_13_01::profile() const
{
   static const std::string Profile("A5-13-01");
   return Profile;
}

const std::string& CProfile_A5_13_01::title() const
{
   static const std::string Title(
      "Environmental Applications - Weather Station");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_13_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_01::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   switch (bitset_extract(data, 24, 4))
   {
   case 1:
      m_dawn->set(static_cast<double>(bitset_extract(data, 0, 8)) * 1000.0 / 255.0);
      m_temperature->set(static_cast<double>(bitset_extract(data, 8, 8)) * 120.0 / 255.0 - 40.0);
      m_windSpeed->set(static_cast<double>(bitset_extract(data, 16, 8)) * 70.0 / 255.0);
      m_dayLight->set(bitset_extract(data, 29, 1) ? true : false);
      m_rain->set(bitset_extract(data, 30, 1) ? true : false);

      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>({
         m_dawn, m_temperature, m_windSpeed, m_dayLight, m_rain
      });
   case 2:
      return m_subProfile_A5_13_02.states(rorg,
                                          data,
                                          status,
                                          senderId,
                                          messageHandler);
   case 6:
      return m_subProfile_A5_13_06.states(rorg,
                                          data,
                                          status,
                                          senderId,
                                          messageHandler);
   default:
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : identifier=" << bitset_extract(data, 24, 4);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }
}

void CProfile_A5_13_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_13_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
