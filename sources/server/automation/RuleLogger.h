#pragma once
#include "IRuleLogger.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rule logger
   //-----------------------------------------------------
   class CRuleLogger : public IRuleLogger
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] logFilePath     Path to target log file
      //-----------------------------------------------------
      explicit CRuleLogger(const boost::filesystem::path& logFilePath);

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

      std::ofstream m_logFile;
   };
} // namespace automation


