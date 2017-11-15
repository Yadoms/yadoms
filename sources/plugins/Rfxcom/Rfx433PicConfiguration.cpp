#include "stdafx.h"
#include "Rfx433PicConfiguration.h"



CRfx433PicConfiguration::CRfx433PicConfiguration()
{
}

CRfx433PicConfiguration::~CRfx433PicConfiguration()
{
}

const std::string& CRfx433PicConfiguration::device() const
{
   static const std::string deviceName("PIC24FJ64GA002");
   return deviceName;
}

unsigned long CRfx433PicConfiguration::deviceType() const
{
   return 2;
}

unsigned long CRfx433PicConfiguration::programAddressStart() const
{
   return 0x001800;
}

unsigned long CRfx433PicConfiguration::programAddressEnd() const
{
   return 0x00A7FF;
}

unsigned long CRfx433PicConfiguration::bytesPerAddr() const
{
   return 2;
}

unsigned long CRfx433PicConfiguration::eraseBlockSize() const
{
   return 2048;
}

unsigned long CRfx433PicConfiguration::readBlockSize() const
{
   return 4;
}

unsigned long CRfx433PicConfiguration::writeBlockSize() const
{
   return 256;
}
