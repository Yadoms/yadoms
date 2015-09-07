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

double CMegatecUpsConfiguration::outuputShutdownDelay() const
{
   return m_data.get<double>("UpsShutdownTimers.content.ShutdownDelay");
}

double CMegatecUpsConfiguration::outuputRestoreDelay() const
{
   return m_data.get<double>("UpsShutdownTimers.content.RestoreDelay");
}
