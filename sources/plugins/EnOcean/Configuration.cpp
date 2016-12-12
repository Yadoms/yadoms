#include "stdafx.h"
#include "Configuration.h"

CConfiguration::~CConfiguration()
{
}

void CConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

std::string CConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}

