#include "stdafx.h"
#include "RfxcomConfiguration.h"

namespace cfg = shared::plugin::configuration;


CRfxcomConfiguration::~CRfxcomConfiguration()
{
}

std::string CRfxcomConfiguration::getSerialPort() const
{
   return getValue<std::string>("Serial Port");
}


