#include "stdafx.h"
#include "ZiBlueTransceiver.h"
#include <shared/Log.h>

CZiBlueTransceiver::CZiBlueTransceiver()
{
}

CZiBlueTransceiver::~CZiBlueTransceiver()
{
}


shared::communication::CByteBuffer CZiBlueTransceiver::buildHelloCmd() const
{
   shared::communication::CByteBuffer buffer(50);
//   memcpy(buffer.begin(), &rbuf, subStructureSize);
   return buffer;
}

