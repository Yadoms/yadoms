#include "stdafx.h"
#include "BufferLogger.h"

namespace shared
{
   namespace communication
   {
      CBufferLogger::CBufferLogger(std::ostream& os)
         : m_os(os)
      {
      }

      CBufferLogger::~CBufferLogger()
      {
      }

      void CBufferLogger::logReceived(const CByteBuffer& data)
      {
         m_os << "Yadoms <<< " << msgToString(data) << std::endl;
      }

      void CBufferLogger::logSent(const CByteBuffer& data)
      {
         m_os << "Yadoms >>> " << msgToString(data) << std::endl;
      }

      std::string CBufferLogger::msgToString(const CByteBuffer& data) const
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


