#include "stdafx.h"
#include "PortLogger.h"
#include <shared/Log.h>


CPortLogger::CPortLogger(const std::string& prefix)
   :m_prefix(prefix)
{
}

CPortLogger::~CPortLogger()
{
}

void CPortLogger::logReceived(const CByteBuffer& data) 
{
   YADOMS_LOG(debug) << m_prefix << " <<< Yadoms : " << msgToString(data);
}

void CPortLogger::logSent(const CByteBuffer& data)
{
   YADOMS_LOG(debug) << "Yadoms >>> " << m_prefix << " : " << msgToString(data);
}

std::string CPortLogger::msgToString(const CByteBuffer& data) const
{
   std::ostringstream ss;
   for (size_t idx = 0 ; idx < data.size() ; ++ idx)
      ss << std::setfill('0') << std::setw(sizeof(CByteBuffer::value_type) * 2) << std::hex << (int)(data.content()[idx]) << " ";

   return ss.str();
}

