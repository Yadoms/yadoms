#include "stdafx.h"
#include "EnOceanConfiguration.h"

CEnOceanConfiguration::~CEnOceanConfiguration()
{
}

void CEnOceanConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

std::string CEnOceanConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}