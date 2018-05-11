#include "stdafx.h"
#include "NitramConfiguration.h"

CNitramConfiguration::~CNitramConfiguration()
{
}

void CNitramConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

std::string CNitramConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}

boost::posix_time::time_duration CNitramConfiguration::outputShutdownDelay() const
{
   boost::posix_time::time_duration delay(boost::posix_time::minutes(m_data.get<int>("UpsShutdownTimers.content.ShutdownDelay")));
   return delay;
}

boost::posix_time::time_duration CNitramConfiguration::outputRestoreDelay() const
{
   boost::posix_time::time_duration delay(boost::posix_time::minutes(m_data.get<int>("UpsShutdownTimers.content.UpsRestoreTimer.content.RestoreDelay")));
   return delay;
}

bool CNitramConfiguration::outputRestoreDelayActivated() const
{
   return m_data.get<bool>("UpsShutdownTimers.content.UpsRestoreTimer.checkbox");
}