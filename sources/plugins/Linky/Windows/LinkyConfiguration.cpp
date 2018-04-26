#include "stdafx.h"
#include "LinkyConfiguration.h"

CLinkyConfiguration::CLinkyConfiguration()
{
}

CLinkyConfiguration::~CLinkyConfiguration()
{
}

void CLinkyConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}

std::string CLinkyConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("SerialPort");
}