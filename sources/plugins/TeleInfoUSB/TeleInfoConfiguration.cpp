#include "stdafx.h"
#include "TeleInfoConfiguration.h"
#include <shared/StringExtension.h>

CTeleInfoConfiguration::CTeleInfoConfiguration()
{
}

CTeleInfoConfiguration::~CTeleInfoConfiguration()
{
}

void CTeleInfoConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);
}

std::string CTeleInfoConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}