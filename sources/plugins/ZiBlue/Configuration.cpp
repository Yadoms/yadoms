#include "stdafx.h"
#include "Configuration.h"

CConfiguration::~CConfiguration()
{
}

void CConfiguration::initializeWith(shared::CDataContainer container)
{
   m_container.initializeWith(container);
}


std::string CConfiguration::getSerialPort() const
{
   return m_container.get<std::string>("Serial Port");
}



