#include "stdafx.h"
#include "Logger.h"
#include <shared/Log.h>
#include <shared/currentTime/Provider.h>


namespace shared
{
   namespace process
   {
      CLogger::CLogger(const std::string& loggerName,
                       const boost::filesystem::path& logFilePath)
         : m_loggerName(loggerName)
      {
         if (!boost::filesystem::exists(logFilePath.parent_path()))
            boost::filesystem::create_directories(logFilePath.parent_path());
         m_logFile.open(logFilePath.string(), std::ofstream::out | std::ofstream::app);
      }

      CLogger::~CLogger()
      {
      }

      void CLogger::init()
      {
         YADOMS_LOG_CONFIGURE(m_loggerName);
      }

      void CLogger::information(const std::string& line)
      {
         auto toPrint = removeEol(line);

         // Write into file
         m_logFile << now() << " : [INFORMATION] " << toPrint << std::endl;

         // Log normally
         YADOMS_LOG(information) << toPrint;
      }

      void CLogger::error(const std::string& line)
      {
         auto toPrint = removeEol(line);

         // Write into file
         m_logFile << now() << " : [ERROR] " << toPrint << std::endl;

         // Log normally
         YADOMS_LOG(error) << toPrint;
      }

      std::string CLogger::now()
      {
         std::stringstream dateStream;
         boost::posix_time::time_facet* facet(new boost::posix_time::time_facet());
         facet->format("%Y/%m/%d %H:%M:%S");
         dateStream.imbue(std::locale(std::locale::classic(), facet));
         dateStream << currentTime::Provider().now();
         return dateStream.str();
      }

      std::string CLogger::removeEol(const std::string& line)
      {
         auto len = line.size();

         if (len > 1 && line[len - 2] == '\r' && line[len - 1] == '\n')
            return line.substr(0, len - 2);
         if (len > 0 && (line[len - 1] == '\r' || line[len - 1] == '\n'))
            return line.substr(0, len - 1);

         return line;
      }
   }
} // namespace shared::process


