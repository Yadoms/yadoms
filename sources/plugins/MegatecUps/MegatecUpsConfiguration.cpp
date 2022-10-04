#include "stdafx.h"
#include "MegatecUpsConfiguration.h"

#include <regex>


void CMegatecUpsConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
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

bool CMegatecUpsConfiguration::autotestEnable() const
{
   return m_data.get<bool>("Autotest.checkbox");
}

boost::posix_time::time_duration CMegatecUpsConfiguration::autotestStartTime() const
{
   const auto hhmm = m_data.get<std::string>("Autotest.content.AutotestStartTime");
   const std::regex pattern("^([0-1]?[0-9]|2[0-3]):([0-5][0-9])$");
   std::smatch match;
   if (!std::regex_match(hhmm, match, pattern))
      throw std::runtime_error("Auto-test start time is invalid : " + hhmm);

   const auto hh = std::stoul(match[1].str());
   const auto mm = std::stoul(match[2].str());

   return {hh, mm, 0};
}

double CMegatecUpsConfiguration::outputShutdownDelay() const
{
   return m_data.get<double>("UpsShutdownTimers.content.ShutdownDelay");
}

double CMegatecUpsConfiguration::outputRestoreDelay() const
{
   return m_data.get<double>("UpsShutdownTimers.content.RestoreDelay");
}
