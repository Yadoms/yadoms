#include "stdafx.h"
#include "Profile_D2_04_Common.h"

#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"


CProfile_D2_04_Common::CProfile_D2_04_Common(double co2SensorRange)
   : m_temperature(boost::make_shared<yApi::historization::CTemperature>("Temperature",
                                                                         yApi::EKeywordAccessMode::kGet)),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>("Humidity",
                                                                   yApi::EKeywordAccessMode::kGet)),
     m_co2(boost::make_shared<specificHistorizers::CConcentrationPpm>("Concentration(ppm)",
                                                                      yApi::EKeywordAccessMode::kGet)),
     m_day(boost::make_shared<yApi::historization::CSwitch>("Day",
                                                            yApi::EKeywordAccessMode::kGet)),
     m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery",
                                                                      yApi::EKeywordAccessMode::kGet)),
     m_co2SensorRange(co2SensorRange)
{
}

void CProfile_D2_04_Common::states(const boost::dynamic_bitset<>& data,
                                   const boost::dynamic_bitset<>& status) const
{
   m_temperature->set(static_cast<double>(bitset_extract(data, 16, 8)) * 51.0 / 255.0);

   m_humidity->set(static_cast<double>(bitset_extract(data, 8, 8)) * 100.0 / 200.0);

   m_co2->set(static_cast<double>(bitset_extract(data, 0, 8)) * m_co2SensorRange / 255.0);

   m_day->set(bitset_extract(data, 24, 1) ? false : true);

   switch (bitset_extract(data, 25, 3))
   {
   case 0:
      m_battery->set(100);
      break;
   case 1:
      m_battery->set(87);
      break;
   case 2:
      m_battery->set(75);
      break;
   case 3:
      m_battery->set(62);
      break;
   case 4:
      m_battery->set(50);
      break;
   case 5:
      m_battery->set(37);
      break;
   case 6:
      m_battery->set(25);
      break;
   default:
      m_battery->set(0);
      break;
   }
}
