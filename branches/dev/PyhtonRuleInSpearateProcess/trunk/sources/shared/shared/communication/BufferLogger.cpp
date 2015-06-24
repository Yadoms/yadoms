#include "stdafx.h"
#include "BufferLogger.h"
#include <shared/Log.h>
#include <shared/StringExtension.h>

namespace shared { namespace communication {

CBufferLogger::CBufferLogger()
{
}

CBufferLogger::~CBufferLogger()
{
}

void CBufferLogger::logReceived(const CByteBuffer& data) 
{
   YADOMS_LOG(debug) << "Yadoms <<< " << msgToString(data);
}

void CBufferLogger::logSent(const CByteBuffer& data)
{
   YADOMS_LOG(debug) << "Yadoms >>> " << msgToString(data);
}

std::string CBufferLogger::msgToString(const CByteBuffer& data) const
{
   if (data.size() == 0)
      return shared::CStringExtension::EmptyString;

   size_t nbDigits = sizeof(data[0]) * 2;
   std::ostringstream ss;
   for (size_t idx = 0 ; idx < data.size() ; ++ idx)
      ss << std::setfill('0') << std::setw(nbDigits) << std::hex << (int)(data[idx]) << " ";

   return ss.str();
}

} } // namespace shared::communication
