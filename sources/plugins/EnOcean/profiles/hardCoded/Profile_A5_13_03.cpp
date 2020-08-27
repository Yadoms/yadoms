#include "stdafx.h"
#include "Profile_A5_13_03.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_A5_13_03::CProfile_A5_13_03(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
   m_date(boost::make_shared<yApi::historization::CDateTime>("Date")),
   m_gpsSource(boost::make_shared<yApi::historization::CSwitch>("GPS source")),
     m_historizers({ m_date, m_gpsSource })
{
}

const std::string& CProfile_A5_13_03::profile() const
{
   static const std::string Profile("A5-13-03");
   return Profile;
}

const std::string& CProfile_A5_13_03::title() const
{
   static const std::string Title("Environmental Applications - Date Exchange");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_03::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_13_03::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_13_03::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (bitset_extract(data, 24, 4) != 3)
   {
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : identifier=" << bitset_extract(data, 24, 4);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   const auto day = static_cast<unsigned short>(bitset_extract(data, 3, 5));
   const auto month = static_cast<unsigned short>(bitset_extract(data, 12, 4));
   const auto year = static_cast<unsigned short>(bitset_extract(data, 17, 7));

   if (day < 1 || day > 31)
   {
      YADOMS_LOG(error) << "Unsupported data received for profile " << profile() <<
         " : day=" << day;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   if (month < 1 || month > 12)
   {
      YADOMS_LOG(error) << "Unsupported data received for profile " << profile() <<
         " : month=" << month;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   if (year < 0 || year > 99)
   {
      YADOMS_LOG(error) << "Unsupported data received for profile " << profile() <<
         " : year=" << year;
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_date->set(boost::posix_time::ptime(boost::gregorian::date(2000 + year, month, day)));

   m_gpsSource->set(bitset_extract(data, 31, 1) ? true : false);

   return m_historizers;
}

void CProfile_A5_13_03::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_13_03::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
