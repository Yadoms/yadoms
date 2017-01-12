#include "stdafx.h"
#include "RuleLogger.h"
#include <shared/Log.h>
#include <shared/currentTime/Provider.h>
#include <shared/shared/StringExtension.h>


namespace automation
{
   CRuleLogger::CRuleLogger(int scriptInstanceId,
                            const boost::filesystem::path& logFilePath)
      : m_prefix("rule #" + std::to_string(scriptInstanceId) + " ")
   {
      if (!boost::filesystem::exists(logFilePath.parent_path()))
         boost::filesystem::create_directories(logFilePath.parent_path());
      m_logFile.open(logFilePath.string(), std::ofstream::out | std::ofstream::app);
   }

   CRuleLogger::~CRuleLogger()
   {
   }

   void CRuleLogger::information(const std::string& line)
   {
      auto toPrint = shared::CStringExtension::removeEol(line);

      // Write into file
      m_logFile << now() << " : [INFORMATION] " << toPrint << std::endl;

      // Log normally
      YADOMS_LOG(information) << m_prefix << toPrint;
   }

   void CRuleLogger::error(const std::string& line)
   {
      auto toPrint = shared::CStringExtension::removeEol(line);

      // Write into file
      m_logFile << now() << " : [ERROR] " << toPrint << std::endl;

      // Log normally
      YADOMS_LOG(error) << m_prefix << toPrint;
   }

   std::string CRuleLogger::now()
   {
      std::stringstream dateStream;
      auto facet(new boost::posix_time::time_facet());
      facet->format("%Y/%m/%d %H:%M:%S");
      dateStream.imbue(std::locale(std::locale::classic(), facet));
      dateStream << shared::currentTime::Provider().now();
      return dateStream.str();
   }
} // namespace automation
