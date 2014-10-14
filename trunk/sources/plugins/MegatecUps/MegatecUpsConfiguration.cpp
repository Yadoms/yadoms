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

unsigned int CMegatecUpsConfiguration::powerLossFilterDelay() const
{
   if (!m_data.get<bool>("PowerLossFilter.checkbox"))
      return 0;

   return m_data.get<unsigned int>("PowerLossFilter.content.Delay");
}
