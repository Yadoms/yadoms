#include "stdafx.h"
#include "RuleLogDispatcher.h"


namespace automation
{
   namespace interpreter
   {
      CRuleLogDispatcher::CRuleLogDispatcher()
      {
      }

      CRuleLogDispatcher::~CRuleLogDispatcher()
      {
      }

      void CRuleLogDispatcher::addLogger(int scriptInstanceId,
                                         boost::shared_ptr<IRuleLogger> ruleLogger)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loggersMutex);
         m_loggers[scriptInstanceId] = ruleLogger;
      }

      void CRuleLogDispatcher::removeLogger(int scriptInstanceId)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loggersMutex);
         const auto& logger = m_loggers.find(scriptInstanceId);
         if (logger != m_loggers.end())
            m_loggers.erase(logger);
      }

      void CRuleLogDispatcher::log(int scriptInstanceId,
                                   bool error,
                                   const std::string& line)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loggersMutex);
         const auto& logger = m_loggers.find(scriptInstanceId);
         if (logger == m_loggers.end())
            return;

         if (error)
            logger->second->error(line);
         else
            logger->second->information(line);
      }
   }
} // namespace automation::interpreter


