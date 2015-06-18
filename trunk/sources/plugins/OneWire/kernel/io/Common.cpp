#include "stdafx.h"
#include "Common.h"
#include <shared/exception/Exception.hpp>

namespace kernel { namespace io {

void CCommon::sendAndReceive(const boost::filesystem::path& deviceFile,
   const unsigned char* cmd, size_t cmdSize, unsigned char* answer, size_t answerSize)
{
   const boost::filesystem::path rwFile = deviceFile / "rw";
   std::fstream file(rwFile.string(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
   if (!file.is_open())
      throw shared::exception::CException("Error opening " + deviceFile.string());

   file.write((char*)cmd, cmdSize);

   file.read((char*)answer, answerSize);
}

unsigned short CCommon::crc16AddByte(unsigned short crc, unsigned char a)
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

unsigned char CCommon::crc16(const unsigned char* byteArray, size_t arraySize)
{
   unsigned short crc16 = 0;
   for (size_t i = 0; i < arraySize; i++)
      crc16 = crc16AddByte(crc16, byteArray[i]);
   return (~crc16) & 0xFF;
}

} } // namespace kernel::io
