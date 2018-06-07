
#include "stdafx.h"
#include "Common.h"


enum { DEVICE_ID_SIZE = 6 };


// str is the family hex code "ff"
EOneWireFamily ToFamily(const std::string& str)
{
   if (str.length() > 2)
      return kUnknown;

   unsigned int xID;
   std::stringstream ss;
   ss << std::hex << str;
   if (!(ss >> xID))
      return kUnknown;

   return static_cast<EOneWireFamily>(xID);
}

std::string ByteArrayToDeviceId(const unsigned char* byteArray)
{
   std::stringstream sID;
   for (size_t idx = DEVICE_ID_SIZE; idx > 0; idx--)
      sID << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (unsigned int)byteArray[idx - 1];
   return sID.str();
}

unsigned short crc16_update(unsigned short crc, unsigned char a)
{
   crc ^= a;
   for (int i = 0; i < 8; ++i)
   {
      if (crc & 1)
         crc = (crc >> 1) ^ 0xA001;
      else
         crc = (crc >> 1);
   }

   return crc;
}

unsigned char Crc16(const unsigned char* byteArray, size_t arraySize)
{
   unsigned short crc16 = 0;
   for (size_t i = 0; i < arraySize; i++)
      crc16 = crc16_update(crc16, byteArray[i]);
   return (~crc16) & 0xFF;
}
