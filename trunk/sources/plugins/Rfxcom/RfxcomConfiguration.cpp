#include "stdafx.h"
#include "RfxcomConfiguration.h"

CRfxcomConfiguration::~CRfxcomConfiguration()
{
}

std::string CRfxcomConfiguration::getSerialPort() const
{
   return get<std::string>("Serial Port");
}


