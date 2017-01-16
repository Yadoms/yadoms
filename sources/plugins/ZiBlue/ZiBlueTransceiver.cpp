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
   shared::communication::CStringBuffer buffer("ZIA++HELLO");
   return buffer;
}

