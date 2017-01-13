#pragma once
#include "IRuleLogger.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rule logger
   //-----------------------------------------------------
   class CRuleLogger : public IRuleLogger //TODO utile ?
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] scriptInstanceId     Script instance ID, used to rpefix logs
      ///\param[in] logFilePath           Path to target log file
      //-----------------------------------------------------
      CRuleLogger(int scriptInstanceId,
                  const boost::filesystem::path& logFilePath);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRuleLogger();

      // IRuleLogger Implementation
      void information(const std::string& line) override;
      void error(const std::string& line) override;
      // [END] IRuleLogger Implementation

   private:
      static std::string now();

      const std::string m_prefix;
      std::ofstream m_logFile;
   };
} // namespace automation
