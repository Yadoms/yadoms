#include "stdafx.h"
#include "ZiBlueConfiguration.h"

CZiBlueConfiguration::~CZiBlueConfiguration()
{
}

void CZiBlueConfiguration::initializeWith(shared::CDataContainer container)
{
   m_container.initializeWith(container);
}


std::string CZiBlueConfiguration::getSerialPort() const
{
   return m_container.get<std::string>("Serial Port");
}



