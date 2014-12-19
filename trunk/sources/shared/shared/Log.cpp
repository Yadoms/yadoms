#include "stdafx.h"
#include "Log.h"
#include "logInternal/LogStreamEnhanced.h"
#include "StringExtension.h"

namespace shared
{
   Poco::Logger& CLog::m_logger = Poco::Logger::get("YadomsLogger");
   std::map<Poco::Thread::TID, std::string> CLog::m_threadNames;

   Poco::Logger& CLog::logger()
   {
      return m_logger;
   }
   
   boost::shared_ptr<logInternal::ILogStream> CLog::logStream()
   {
      return boost::shared_ptr<logInternal::ILogStream>(new logInternal::CLogStreamEnhanced(CLog::m_logger));
   }

   void CLog::setThreadName(const std::string & name)
   {
      std::map<Poco::Thread::TID, std::string>::iterator i = m_threadNames.find(Poco::Thread::currentTid());
      if (i != m_threadNames.end())
         m_threadNames.erase(i);
         
      m_threadNames.insert(std::make_pair(Poco::Thread::currentTid(), name));
   }

   const std::string & CLog::getCurrentThreadName()
   {
      std::map<Poco::Thread::TID, std::string>::iterator i = m_threadNames.find(Poco::Thread::currentTid());
      if (i != m_threadNames.end())
         return i->second;
      return CStringExtension::EmptyString;
   }
} // namespace shared