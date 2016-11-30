#include "stdafx.h"
#include "NoBufferLogger.h"

namespace shared
{
   namespace communication
   {
      CNoBufferLogger::CNoBufferLogger()
      {
      }

      CNoBufferLogger::~CNoBufferLogger()
      {
      }

      void CNoBufferLogger::logReceived(const CByteBuffer&)
      {
      }

      void CNoBufferLogger::logSent(const CByteBuffer&)
      {
      }
   }
} // namespace shared::communication


