#include "stdafx.h"
#include "InterpreterLogger.h"
#include <shared/Log.h>


namespace logging
{
   CInterpreterLogger::CInterpreterLogger(const std::string& loggerName)
      : CYadomsSubModuleProcessLogger(loggerName)
   {
   }

   CInterpreterLogger::~CInterpreterLogger()
   {
   }

   std::string CInterpreterLogger::extractMessage(const std::string& line,
                                                  std::string& outMessage) const
   {
      // Line is formatted like : "[LogLevel][Script]Message"
      boost::regex pattern("\\[(.*)\\]\\[(.*)\\](.*)");
      boost::smatch result;
      if (!boost::regex_search(line, result, pattern))
         return std::string();

      outMessage = std::string(result[2].first, result[2].second);
      return std::string(result[1].first, result[1].second);
   }
} // namespace logging


