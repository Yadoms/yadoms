#include "stdafx.h"
#include "NitramHelpers.h"
#include <shared/exception/Exception.hpp>

unsigned char calculateCheckSum(const shared::communication::CByteBuffer& message, unsigned char size)
{
   const unsigned char poly8 = 0xD5;
   unsigned char crc = 0;
   unsigned char i;
   size_t position = 0;

   while (position < size)
   {
      crc = crc ^ (message[position]);
      for (i = 0; i < 8; i++)
      {
         if (crc & 0x80)
            crc = (crc << 1) ^ poly8;
         else
            crc <<= 1;
      }
      ++position;
   }
   return crc;
}

unsigned char readHeaderPayloadSize(unsigned char header)
{
   return (header & 0x3F);
}

bool readHeaderError(unsigned char header)
{
   return ((header & 0x80) != 0);
}

bool readHeaderFinish(unsigned char header)
{
   return ((header & 0x40) != 0);
}

unsigned char createHeader(unsigned char size,
                           bool error,
                           bool finish)
{
   // Calcul de la taille
   unsigned char temp = size & 0x3F;

   // Ajout des autres éléments
   if (error)
      temp |= 0x80;

   if (finish)
      temp |= 0x40;

   return temp;
}