#include "stdafx.h"
#include "BufferLogger.h"
#include <shared/Log.h>


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
   std::ostringstream ss;
   for (size_t idx = 0 ; idx < data.size() ; ++ idx)
      ss << std::setfill('0') << std::setw(sizeof(CByteBuffer::value_type) * 2) << std::hex << (int)(data.content()[idx]) << " ";

   return ss.str();
}

