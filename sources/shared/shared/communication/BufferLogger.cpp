#include "stdafx.h"
#include "BufferLogger.h"
#include "BufferLoggerHelper.h"

namespace shared
{
   namespace communication
   {
      CBufferLogger::CBufferLogger(const std::string& logLevel)
         : m_logLevel(CBufferLoggerHelper::fromLoglevelString(logLevel))
      {
      }

      CBufferLogger::~CBufferLogger()
      {
      }

      void CBufferLogger::logReceived(const CByteBuffer& data)
      {
         CBufferLoggerHelper::log(m_logLevel,
                                  "Yadoms <<< " + msgToString(data));
      }

      void CBufferLogger::logSent(const CByteBuffer& data)
      {
         CBufferLoggerHelper::log(m_logLevel,
                                  "Yadoms >>> " + msgToString(data));
      }

      std::string CBufferLogger::msgToString(const CByteBuffer& data)
      {
         if (data.size() == 0)
            return std::string();

         auto nbDigits = sizeof(data[0]) * 2;
         std::ostringstream ss;
         for (size_t idx = 0; idx < data.size(); ++ idx)
            ss << std::setfill('0') << std::setw(nbDigits) << std::hex << static_cast<int>(data[idx]) << " ";

         return ss.str();
      }
   }
} // namespace shared::communication


