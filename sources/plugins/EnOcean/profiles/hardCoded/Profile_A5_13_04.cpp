#include "stdafx.h"
#include "Profile_A5_13_04.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_13_04::CProfile_A5_13_04(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_dayOfWeek(boost::make_shared<specificHistorizers::CDayOfWeek>("DayOfWeek")),
     m_time(boost::make_shared<yApi::historization::CDuration>("TimeOfDay")),
     m_gpsSource(boost::make_shared<yApi::historization::CSwitch>("GPS source")),
     m_historizers({m_dayOfWeek, m_time, m_gpsSource})
{
}

const std::string& CProfile_A5_13_04::profile() const
{
   static const std::string Profile("A5-13-04");
   return Profile;
}

const std::string& CProfile_A5_13_04::title() const
{
   static const std::string Title("Environmental Applications - Time and Day Exchange");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_04::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_13_04::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_04::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (bitset_extract(data, 24, 4) != 4)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : identifier=" << bitset_extract(data, 24, 4);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_dayOfWeek->set(specificHistorizers::EDayOfWeek(bitset_extract(data, 0, 3)));

   auto hour = static_cast<unsigned short>(bitset_extract(data, 3, 5));
   const auto minute = static_cast<unsigned short>(bitset_extract(data, 10, 6));
   const auto second = static_cast<unsigned short>(bitset_extract(data, 18, 6));
   const auto amPmTimeFormat = bitset_extract(data, 30, 1) ? true : false;

   if (amPmTimeFormat && (hour < 1 || hour > 12) ||
      !amPmTimeFormat && (hour < 0 || hour > 23))
   {
      YADOMS_LOG(error) << "Unsupported data received for profile " << profile() <<
         " : hour=" << hour;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }
   if (amPmTimeFormat)
      hour = convertTo24HFormat(hour, bitset_extract(data, 30, 1) ? true : false);

   if (minute < 0 || minute > 59)
   {
      YADOMS_LOG(error) << "Unsupported data received for profile " << profile() <<
         " : minute=" << minute;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   if (second < 0 || second > 59)
   {
      YADOMS_LOG(error) << "Unsupported data received for profile " << profile() <<
         " : second=" << second;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_time->set(static_cast<double>(hour * 24 * 60 + minute * 60 + second));

   m_gpsSource->set(bitset_extract(data, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_13_04::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_13_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}

unsigned short CProfile_A5_13_04::convertTo24HFormat(int amPmHour,
                                                     bool isPm)
{
   if (isPm)
   {
      // PM
      if (amPmHour == 12)
         return amPmHour;
      return amPmHour + 12;
   }
   // AM
   if (amPmHour == 12)
      return 0;
   return amPmHour;
}
