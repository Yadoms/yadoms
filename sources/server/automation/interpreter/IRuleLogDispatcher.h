#pragma once
#include <server/automation/IRuleLogger.h>

namespace automation
{
   namespace interpreter
   {
      //-----------------------------------------------------
      ///\brief The rule log dispatcher
      //-----------------------------------------------------
      class IRuleLogDispatcher
      {
      public:
         virtual ~IRuleLogDispatcher()
         {
         }

         virtual void addLogger(int scriptInstanceId,
                                boost::shared_ptr<IRuleLogger> ruleLogger) = 0;

         virtual void removeLogger(int scriptInstanceId) = 0;

         virtual void log(int scriptInstanceId,
                          bool error,
                          const std::string& line) = 0;
      };
   }
} // namespace automation::interpreter


