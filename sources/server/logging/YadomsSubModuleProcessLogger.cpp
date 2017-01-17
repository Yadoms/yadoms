#include "stdafx.h"
#include "YadomsSubModuleProcessLogger.h"
#include <shared/Log.h>
#include <shared/StringExtension.h>


namespace logging
{
   CYadomsSubModuleProcessLogger::CYadomsSubModuleProcessLogger(const std::string& loggerName)
      : m_loggerName(loggerName),
        m_logger(Poco::Logger::get(m_loggerName))
   {
   }

   CYadomsSubModuleProcessLogger::~CYadomsSubModuleProcessLogger()
   {
   }

   void CYadomsSubModuleProcessLogger::init()
   {
      YADOMS_LOG_CONFIGURE(m_loggerName);
   }

   void CYadomsSubModuleProcessLogger::information(const std::string& line)
   {
      auto message = line;
      auto logLevel = extractMessage(shared::CStringExtension::removeEol(line),
                                     message);
      doLog(logLevel.empty() ? "Information" : logLevel,
            message);
   }

   void CYadomsSubModuleProcessLogger::error(const std::string& line)
   {
      YADOMS_LOG(error) << shared::CStringExtension::removeEol(line);
   }

   std::string CYadomsSubModuleProcessLogger::extractMessage(const std::string& line,
                                                             std::string& outMessage) const
   {
      // Line is formatted like : "[LogLevel]Message"
      boost::regex pattern("\\[(.*)\\](.*)");
      boost::smatch result;
      if (!boost::regex_search(line, result, pattern))
         return std::string();

      outMessage = std::string(result[2].first, result[2].second);
      return std::string(result[1].first, result[1].second);
   }

   void CYadomsSubModuleProcessLogger::doLog(const std::string& logLevel,
                                             const std::string& message)
   {
      if (logLevel == "Fatal")
      YADOMS_LOG(fatal) << message;
      else if (logLevel == "Critical")
      YADOMS_LOG(critical) << message;
      else if (logLevel == "Error")
      YADOMS_LOG(error) << message;
      else if (logLevel == "Warning")
      YADOMS_LOG(warning) << message;
      else if (logLevel == "Notice")
      YADOMS_LOG(notice) << message;
      else if (logLevel == "Information")
      YADOMS_LOG(information) << message;
      else if (logLevel == "Debug")
      YADOMS_LOG(debug) << message;
      else if (logLevel == "Trace")
      YADOMS_LOG(trace) << message;
      else
      YADOMS_LOG(error) << message;
   }
} // namespace logging


