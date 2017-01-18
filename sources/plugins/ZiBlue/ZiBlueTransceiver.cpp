#include "stdafx.h"
#include "ZiBlueTransceiver.h"
#include <shared/Log.h>

CZiBlueTransceiver::CZiBlueTransceiver()
{
}

CZiBlueTransceiver::~CZiBlueTransceiver()
{
}


std::string CZiBlueTransceiver::buildHelloCmd() const
{
   return "ZIA++HELLO\r\n";
}


std::string CZiBlueTransceiver::buildStartListeningData() const
{
   return "ZIA++FORMAT JSON\r\n";
}

