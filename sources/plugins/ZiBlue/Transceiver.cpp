#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>

CTransceiver::CTransceiver()
{
}

CTransceiver::~CTransceiver()
{
}


std::string CTransceiver::buildHelloCmd() const
{
   return "ZIA++HELLO\r\n";
}


std::string CTransceiver::buildStartListeningData() const
{
   return "ZIA++FORMAT JSON\r\n";
}

