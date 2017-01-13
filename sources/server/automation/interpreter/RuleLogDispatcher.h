#pragma once
#include "IRuleLogDispatcher.h"

namespace automation
{
   namespace interpreter
   {
      //-----------------------------------------------------
      ///\brief The rule log dispatcher
      //-----------------------------------------------------
      class CRuleLogDispatcher : public IRuleLogDispatcher
      {
      public:
         CRuleLogDispatcher();
         virtual ~CRuleLogDispatcher();

         // IRuleLogDispatcher Implementation
         void addLogger(int scriptInstanceId,
                        boost::shared_ptr<IRuleLogger> ruleLogger) override;
         void removeLogger(int scriptInstanceId) override;
         void log(int scriptInstanceId,
                  bool error,
                  const std::string& line) override;
         // [END] IRuleLogDispatcher Implementation

      private:
         mutable boost::recursive_mutex m_loggersMutex;
         std::map<int, boost::shared_ptr<IRuleLogger>> m_loggers;
      };
   }
} // namespace automation::interpreter


