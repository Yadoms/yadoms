#include "stdafx.h"
#include "Profile_A5_11_01.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_11_01::CProfile_A5_11_01(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_illumination(boost::make_shared<yApi::historization::CIllumination>("Illumination")),
     m_illuminationSetPoint(boost::make_shared<yApi::historization::CIllumination>("IlluminationSetPoint")),
     m_dimOutLevel(boost::make_shared<yApi::historization::CDimmable>("IlluminationSetPoint")),
     m_repeater(boost::make_shared<yApi::historization::CSwitch>("Repeater")),
     m_powerRelayTimer(boost::make_shared<yApi::historization::CSwitch>("PowerRelayTimer")),
     m_daylightHarvesting(boost::make_shared<yApi::historization::CSwitch>("DaylightHarvesting")),
     m_dimming(boost::make_shared<yApi::historization::CSwitch>("Dimming")),
     m_magnetContact(boost::make_shared<yApi::historization::CSwitch>("MagnetContact")),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy")),
     m_powerRelay(boost::make_shared<yApi::historization::CSwitch>("PowerRelay")),
     m_historizers({
        m_illumination, m_illuminationSetPoint, m_dimOutLevel, m_repeater, m_powerRelayTimer, m_daylightHarvesting,
        m_dimming, m_magnetContact, m_occupancy, m_powerRelay
     })
{
}

const std::string& CProfile_A5_11_01::profile() const
{
   static const std::string Profile("A5-11-01");
   return Profile;
}

const std::string& CProfile_A5_11_01::title() const
{
   static const std::string Title(
      "Controller Status - Lighting Controller");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_11_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_01::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   m_illumination->set(static_cast<double>(bitset_extract(data, 0, 8)) * 510.0 / 255.0);

   m_illuminationSetPoint->set(static_cast<double>(bitset_extract(data, 8, 8)));

   m_dimOutLevel->set(static_cast<int>(bitset_extract(data, 16, 8)));

   m_repeater->set(bitset_extract(status, 24, 1) ? true : false);

   m_powerRelayTimer->set(bitset_extract(status, 25, 1) ? true : false);

   m_daylightHarvesting->set(bitset_extract(status, 26, 1) ? true : false);

   m_dimming->set(bitset_extract(status, 27, 1) ? true : false);

   m_magnetContact->set(bitset_extract(status, 29, 1) ? true : false);

   m_occupancy->set(bitset_extract(status, 30, 1) ? true : false);

   m_powerRelay->set(bitset_extract(status, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_11_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_11_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
