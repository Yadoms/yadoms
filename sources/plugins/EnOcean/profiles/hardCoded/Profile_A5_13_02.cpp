#include "stdafx.h"
#include "Profile_A5_13_02.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"
#include <algorithm>

CProfile_A5_13_02::CProfile_A5_13_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_sunWest(boost::make_shared<yApi::historization::CIllumination>("SunWest")),
     m_sunNorthSouth(boost::make_shared<yApi::historization::CIllumination>("SunNorthSouth")),
     m_sunEast(boost::make_shared<yApi::historization::CIllumination>("SunEast")),
     m_southHemisphere(boost::make_shared<yApi::historization::CSwitch>("SouthHemisphere", yApi::EKeywordAccessMode::kGet)),
     m_historizers({ m_sunWest, m_sunNorthSouth, m_sunEast, m_southHemisphere })
{
}

const std::string& CProfile_A5_13_02::profile() const
{
   static const std::string Profile("A5-13-02");
   return Profile;
}

const std::string& CProfile_A5_13_02::title() const
{
   static const std::string Title(
      "Environmental Applications - Sun Intensity");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_02::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_13_02::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_02::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (bitset_extract(data, 24, 4) != 2)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : identifier=" << bitset_extract(data, 24, 4);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_sunWest->set(static_cast<double>(bitset_extract(data, 0, 8)) * 150000.0 / 255.0);
   m_sunNorthSouth->set(static_cast<double>(bitset_extract(data, 8, 8)) * 150000.0 / 255.0);
   m_sunEast->set(static_cast<double>(bitset_extract(data, 16, 8)) * 150000.0 / 255.0);

   m_southHemisphere->set(bitset_extract(data, 29, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_13_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_13_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
