#include "stdafx.h"
#include "Profile_D2_03_0A.h"

#include <utility>
#include "profiles/bitsetHelpers.hpp"

CProfile_D2_03_0A::CProfile_D2_03_0A(std::string deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(std::move(deviceId)),
     m_simplePress(boost::make_shared<yApi::historization::CEvent>("SimplePress", yApi::EKeywordAccessMode::kGet)),
     m_doublePress(boost::make_shared<yApi::historization::CEvent>("DoublePress", yApi::EKeywordAccessMode::kGet)),
     m_longPress(boost::make_shared<yApi::historization::CEvent>("LongPress", yApi::EKeywordAccessMode::kGet)),
     m_longPressReleased(boost::make_shared<yApi::historization::CEvent>("LongPressReleased", yApi::EKeywordAccessMode::kGet)),
     m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_simplePress, m_doublePress, m_longPress, m_longPressReleased, m_battery})
{
}

const std::string& CProfile_D2_03_0A::profile() const
{
   static const std::string Profile("D2-03-0A");
   return Profile;
}

const std::string& CProfile_D2_03_0A::title() const
{
   static const std::string Title(R"(Light, Switching + Blind Control - Push Button - Single Button)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_03_0A::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_03_0A::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler)
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_03_0A::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   enum class EEvent
   {
      kSimplePress = 1,
      kDoublePress = 2,
      kLongPress= 3,
      kLongPressReleased = 4
   };

   switch (static_cast<EEvent>(bitset_extract(data, 8, 8)))
   {
   case EEvent::kSimplePress:
      historizers.emplace_back(m_simplePress);
      break;
   case EEvent::kDoublePress:
      historizers.emplace_back(m_doublePress);
      break;
   case EEvent::kLongPress:
      historizers.emplace_back(m_longPress);
      break;
   case EEvent::kLongPressReleased:
      historizers.emplace_back(m_longPressReleased);
      break;
   default:  // NOLINT(clang-diagnostic-covered-switch-default)
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : Event=" << bitset_extract(data, 8, 8);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_battery->set(static_cast<int>(bitset_extract(data, 0, 8)));
   historizers.emplace_back(m_battery);

   return historizers;
}

void CProfile_D2_03_0A::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) {
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_03_0A::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler)
{
   // Device supports no configuration
}
