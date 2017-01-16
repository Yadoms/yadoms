#include "stdafx.h"
#include "ZiBlueTransceiver.h"
#include <shared/Log.h>

CZiBlueTransceiver::CZiBlueTransceiver()
{
}

CZiBlueTransceiver::~CZiBlueTransceiver()
{
}


shared::communication::CStringBuffer CZiBlueTransceiver::buildHelloCmd() const
{
   shared::communication::CStringBuffer buffer("ZIA++HELLO\r\n\0");
   return buffer;
}

