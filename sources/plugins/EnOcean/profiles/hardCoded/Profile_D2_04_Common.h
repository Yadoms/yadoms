#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/smart_ptr/shared_ptr.hpp>

#include "specificHistorizers/ConcentrationPpm.h"


class CProfile_D2_04_Common
{
public:
   CProfile_D2_04_Common(double co2SensorRange);
   virtual ~CProfile_D2_04_Common() = default;

   void states(const boost::dynamic_bitset<>& data,
               const boost::dynamic_bitset<>& status) const;

   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
   boost::shared_ptr<yApi::historization::CHumidity> m_humidity;
   boost::shared_ptr<specificHistorizers::CConcentrationPpm> m_co2;
   boost::shared_ptr<yApi::historization::CSwitch> m_day;
   boost::shared_ptr<yApi::historization::CBatteryLevel> m_battery;

private:
   const double m_co2SensorRange;
};
