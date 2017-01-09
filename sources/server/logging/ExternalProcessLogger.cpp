#include "stdafx.h"
#include "ExternalProcessLogger.h"
#include <shared/Log.h>
#include <shared/currentTime/Provider.h>


namespace logging
{
   CExternalProcessLogger::CExternalProcessLogger(const std::string& loggerName,
                                                  const boost::filesystem::path& logFilePath)
      : m_loggerName(loggerName)
   {
      if (!boost::filesystem::exists(logFilePath.parent_path()))
         boost::filesystem::create_directories(logFilePath.parent_path());
      m_logFile.open(logFilePath.string(), std::ofstream::out | std::ofstream::app);
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
      auto toPrint = removeEol(line);

      // Write into file
      m_logFile << now() << " : [INFORMATION] " << toPrint << std::endl;

      // Log normally
      YADOMS_LOG(information) << toPrint;
   }

   void CExternalProcessLogger::error(const std::string& line)
   {
      auto toPrint = removeEol(line);

      // Write into file
      m_logFile << now() << " : [ERROR] " << toPrint << std::endl;

      // Log normally
      YADOMS_LOG(error) << toPrint;
   }

   std::string CExternalProcessLogger::now()
   {
      std::stringstream dateStream;
      auto facet(new boost::posix_time::time_facet());
      facet->format("%Y/%m/%d %H:%M:%S");
      dateStream.imbue(std::locale(std::locale::classic(), facet));
      dateStream << shared::currentTime::Provider().now();
      return dateStream.str();
   }

   std::string CExternalProcessLogger::removeEol(const std::string& line)
   {
      auto len = line.size();

      if (len > 1 && line[len - 2] == '\r' && line[len - 1] == '\n')
         return line.substr(0, len - 2);
      if (len > 0 && (line[len - 1] == '\r' || line[len - 1] == '\n'))
         return line.substr(0, len - 1);

      return line;
   }
} // namespace logging


