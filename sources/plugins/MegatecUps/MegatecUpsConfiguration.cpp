#include "stdafx.h"
#include "MegatecUpsConfiguration.h"

CMegatecUpsConfiguration::~CMegatecUpsConfiguration()
{
}

void CMegatecUpsConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);
}

std::string CMegatecUpsConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}

bool CMegatecUpsConfiguration::upsBeepEnable() const
{
   return m_data.get<bool>("UpsBeep");
}

IMegatecUpsConfiguration::EPowerFailureManagement CMegatecUpsConfiguration::powerFailureManagement() const
{
   if (m_data.get<bool>("PowerFailureManagement.content.Immediately.radio"))
      return IMegatecUpsConfiguration::kNotifyImmediately;
   else if (m_data.get<bool>("PowerFailureManagement.content.Filter.radio"))
      return IMegatecUpsConfiguration::kFilter;
   else if (m_data.get<bool>("PowerFailureManagement.content.LowBattery.radio"))
      return IMegatecUpsConfiguration::kLowBattery;
   else if (m_data.get<bool>("PowerFailureManagement.content.RemainingBattery.radio"))
      return IMegatecUpsConfiguration::kRemainingBattery;
   else
      throw shared::exception::CInvalidParameter("PowerFailureManagement");
}

unsigned int CMegatecUpsConfiguration::powerFailureFilterDelay() const
{
   BOOST_ASSERT_MSG(powerFailureManagement() == IMegatecUpsConfiguration::kFilter, "Wrong powerFailureManagement choice");
   return m_data.get<unsigned int>("PowerFailureManagement.content.Filter.content.Delay");
}

double CMegatecUpsConfiguration::powerFailureRemainingBatteryThreshold() const
{
   BOOST_ASSERT_MSG(powerFailureManagement() == IMegatecUpsConfiguration::kRemainingBattery, "Wrong powerFailureManagement choice");
   return m_data.get<double>("PowerFailureManagement.content.RemainingBattery.content.Threshold");
}

double CMegatecUpsConfiguration::outuputShutdownDelay() const
{
   return m_data.get<double>("UpsShutdownTimers.content.ShutdownDelay");
}

double CMegatecUpsConfiguration::outuputRestoreDelay() const
{
   return m_data.get<double>("UpsShutdownTimers.content.RestoreDelay");
}
