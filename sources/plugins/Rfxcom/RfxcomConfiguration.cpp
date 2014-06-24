#include "stdafx.h"
#include "RfxcomConfiguration.h"

CRfxcomConfiguration::~CRfxcomConfiguration()
{
}

void CRfxcomConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);
}

std::string CRfxcomConfiguration::getSerialPort() const
{
   return m_data.get<std::string>("Serial Port");
}


