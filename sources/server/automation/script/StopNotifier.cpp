#include "stdafx.h"
#include "StopNotifier.h"
#include "../IRuleStateHandler.h"

namespace automation
{
   namespace script
   {
      StopNotifier::StopNotifier(boost::shared_ptr<IRuleStateHandler> ruleStateHandler,
                                 int ruleId)
         : m_ruleStateHandler(ruleStateHandler),
           m_ruleId(ruleId)
      {
      }

      StopNotifier::~StopNotifier()
      {
      }

      void StopNotifier::onEndOfProcess(int returnCode, const std::string& error)
      {
         if (returnCode == 0)
            notifyNormalStop();
         else
            notifyError(error);
      }

      void StopNotifier::notifyNormalStop()
      {
         m_ruleStateHandler->signalNormalRuleStop(m_ruleId);
      }

      void StopNotifier::notifyError(const std::string& error)
      {
         m_ruleStateHandler->signalRuleError(m_ruleId, error);
      }

      void StopNotifier::notifyStartError(const std::string& error)
      {
         m_ruleStateHandler->signalRulesStartError(m_ruleId, error);
      }
   }
} // namespace automation::script
