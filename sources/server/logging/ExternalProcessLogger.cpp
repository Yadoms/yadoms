#include "stdafx.h"
#include "ExternalProcessLogger.h"
#include <shared/Log.h>
#include <shared/StringExtension.h>


namespace logging
{
   CExternalProcessLogger::CExternalProcessLogger(const std::string& loggerName)
      : m_loggerName(loggerName),
        m_logger(Poco::Logger::get(m_loggerName))
   {
   }

   CExternalProcessLogger::~CExternalProcessLogger()
   {
   }

   void CExternalProcessLogger::init()
   {
      YADOMS_LOG_CONFIGURE(m_loggerName);
   }

   void CExternalProcessLogger::information(const std::string& line)
   {
      YADOMS_LOG(information) << shared::CStringExtension::removeEol(line);
   }

   void CExternalProcessLogger::error(const std::string& line)
   {
      YADOMS_LOG(error) << shared::CStringExtension::removeEol(line);
   }
} // namespace logging


